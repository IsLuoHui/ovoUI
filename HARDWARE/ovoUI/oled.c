#include "oled.h"
#include "spi.h"
#include <stdlib.h>
#include "string.h"


// **全局 OLED 显示缓冲区**
u8 framebuffer[1024] = {0};

ELEMENT *elementPtrs[MAX_ELEMENTS] = {0}; // 存储动态创建的 ELEMENT 指针
u8 elementCount = 0; // 当前元素数量

// **创建 ELEMENT**
ELEMENT OLED_Create_Element(u8 x, u8 y, u8 w, u8 h,u8 mix, u8 *data) {
    ELEMENT ele;
    ele.x = x;
    ele.y = y;
    ele.w = (x + w > WIDTH) ? WIDTH - x : w;
    ele.h = (y + h > HEIGHT) ? HEIGHT - y : h;
    ele.mix = mix;
    ele.data = data;
    return ele;
}

// **添加 ELEMENT 到链表**
void addElement(Node** head, ELEMENT newElement) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return;
    newNode->ele = newElement;
    newNode->next = *head;
    *head = newNode;
}

// **修改 ELEMENT**
void modifyElement(ELEMENT *ele, u8 x, u8 y/*, u8 w, u8 h, u8 *data, u8 mix*/) {
    if (!ele) return;
    
    ele->x = x;
    ele->y = y;
    //ele->w = (x + w > WIDTH) ? WIDTH - x : w;
    //ele->h = (y + h > HEIGHT) ? HEIGHT - y : h;
    //ele->data = data;
    //ele->mix = mix;
}

void modifyElementByData(Node *head, u8 *data, u8 x, u8 y) {
    while (head) {
        if (head->ele.data == data) {
            head->ele.x = x;
            head->ele.y = y;
            return;
        }
        head = head->next;
    }
}

// **删除指定坐标的 ELEMENT**
void removeElement(Node** head, u8 x, u8 y) {
    Node* temp = *head;
    Node* prev = NULL;

    while (temp != NULL) {
        if (temp->ele.x == x && temp->ele.y == y) {
            if (prev == NULL) *head = temp->next;
            else prev->next = temp->next;
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

// **遍历链表并传入 OLED_Mix_Print**
void printElements(Node* head) {
    ELEMENT elements[5]; // 假设最多存储 20 个元素
    u8 count = 0;

    while (head && count < 5) {
        elements[count++] = head->ele;
        head = head->next;
    }

    OLED_Mix_Print(elements, count);
}

// **OLED_Mix_Print 处理多个 ELEMENT**
// void OLED_Mix_Print(ELEMENT* ele, u8 num) {
//     for (u8 i = 0; i < num; i++) {
//         ELEMENT e = ele[i];
//         u8 y_offset = e.y % 8; // 页内偏移
//         u8 page_start = e.y / 8;
//         u8 page_num = (e.h + y_offset + 7) / 8; // 需要写入的页数

//         for (u8 col = 0; col < e.w; col++) {
//             for (u8 page = 0; page < page_num; page++) {
//                 u8 data_low = 0, data_high = 0;
//                 // 取原始数据
//                 u16 data_index = page * e.w + col;
//                 if (data_index < e.h / 8 * e.w) data_low = e.data[data_index];
//                 if (data_index + e.w < e.h / 8 * e.w) data_high = e.data[data_index + e.w];

//                 // 位移合成，处理页内偏移
//                 u8 out = (data_low >> y_offset) | (data_high << (8 - y_offset));
//                 u16 fb_index = (page_start + page) * WIDTH + e.x + col;
//                 if (fb_index < sizeof(framebuffer)) {
//                     if (e.mix == 0x00) framebuffer[fb_index] = out;
//                     else if (e.mix == 0x01) framebuffer[fb_index] |= out;
//                     else if (e.mix == 0x02) framebuffer[fb_index] ^= out;
//                 }
//             }
//         }
//     }
//     OLED_Update();
// }

void OLED_Mix_Print(ELEMENT *ele, u8 num) {
    memset(framebuffer, 0, 1024);
    for (u8 n = 0; n < num; n++) {
        ELEMENT e = ele[n];
        //u8 offset_pix = e.y % 8;
        u8 offset_row = e.y / 8;
        u16 all_row = e.h % 8 == 0 ? e.h / 8 : e.h / 8 + 1;

        
        if (0) { //非整页偏移
            
        }
        else { //整页偏移
            u16 index = 0;
            for (u8 trow = 0;trow < all_row;trow++) {
                for (u8 tcol = 0;tcol < e.w;tcol++) {
                    if (e.x + tcol >= WIDTH) {
                        index++;
                        continue; // 超出屏幕宽度直接跳过
                    }
                    framebuffer[(offset_row + trow) * WIDTH + e.x + tcol] = e.data[index];
                    index++;
                }
            }
            OLED_GShowNum(0, 6, e.x, 3, framebuffer, 1);
            // OLED_GShowNum(0, 4, e.h/8, 8, framebuffer, 1);
            //OLED_GShowNum(0, 6, all_row, 8, framebuffer, 1);
        }

    }
    OLED_RAM_Refresh(framebuffer);
}

// **刷新 OLED**
void OLED_Update() {
    for (u8 page = 0; page < 8; page++) {
        OLED_Set_Cursor(0, page);
        for (u8 col = 0; col < WIDTH; col++) {
            OLED_W_DATA(framebuffer[page * WIDTH + col]);
        }
    }
}
