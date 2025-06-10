#include "oled.h"
#include "spi.h"
#include <stdlib.h>
#include "string.h"


// **全局 OLED 显示缓冲区**
u8 framebuffer[1024] = {0};

ELEMENT *elementPtrs[MAX_ELEMENTS] = {0}; // 存储动态创建的 ELEMENT 指针
u8 elementCount = 0; // 当前元素数量


// **创建 ELEMENT（动态分配）**
ELEMENT *OLED_Create_Element(int16_t x, int16_t y, u8 w, u8 h,u8 mix, u8 *data) {
    if (elementCount >= MAX_ELEMENTS) return NULL; // 超出最大元素限制

    ELEMENT *ele = (ELEMENT *)malloc(sizeof(ELEMENT));
    if (!ele) return NULL; // 分配失败

    ele->x = x;
    ele->y = y;
    ele->w = (x + w > WIDTH) ? WIDTH - x : w;
    ele->h = (y + h > HEIGHT) ? HEIGHT - y : h;
    ele->mix = mix;
    ele->data = data;

    elementPtrs[elementCount++] = ele; // 存入指针数组
    return ele;
}

// **修改 ELEMENT**
void modifyElement(ELEMENT *ele, int16_t x, int16_t y, u8 w, u8 h, u8 *data, u8 mix) {
    if (!ele) return;
    ele->x = x;
    ele->y = y;
    //ele->w = (x + w > WIDTH) ? WIDTH - x : w;
    //ele->h = (y + h > HEIGHT) ? HEIGHT - y : h;
    //ele->data = data;
    //ele->mix = mix;
}

// **删除 ELEMENT**
void removeElement(u8 index) {
    if (index >= elementCount || !elementPtrs[index]) return;

    free(elementPtrs[index]); // 释放内存
    for (u8 i = index; i < elementCount - 1; i++) {
        elementPtrs[i] = elementPtrs[i + 1]; // 移动指针
    }
    elementPtrs[elementCount - 1] = NULL;
    elementCount--;
}

/**
 * x y w h mix data
 */
void OLED_Mix_Print() {
    memset(framebuffer, 0, 1024);
    for (u8 n = 0; n < elementCount; n++) {
        ELEMENT *e = elementPtrs[n];
        if (!e) continue;

        //u8 offset_pix = e.y % 8;
        //u8 offset_row = e->y / 8;
        u16 all_row = e->h % 8 == 0 ? e->h / 8 : e->h / 8 + 1;

        if (e->w == 0)continue; //没有宽度
        if (e->x >= WIDTH)continue;  //x大于右边界情况
        if (e->x + e->w < 0)continue; //x+w小于左边界情况
        if (e->x >= 0 && e->x + e->w <= WIDTH) {  //范围内情况
            for (u8 trow = 0;trow < all_row;trow++) {
                for (u8 tcol = 0;tcol < e->w;tcol++) {
                    framebuffer[trow * WIDTH + e->x + tcol] = e->data[trow * e->w + tcol];
                }
            }
            OLED_GShowChar(32, 6, '*', framebuffer, 1);
        }
        else if (e->x >= 0 && e->x + e->w > WIDTH) {  //右侧超出部分情况
            u8 draw_w = WIDTH - e->x; // 实际可显示的宽度
            if (draw_w > e->w) draw_w = e->w;
            for (u8 trow = 0; trow < all_row; trow++) {
                for (u8 tcol = 0; tcol < draw_w; tcol++) {
                    framebuffer[trow * WIDTH + e->x + tcol] = e->data[trow * e->w + tcol];
                }
            }
            OLED_GShowChar(32, 6, '+', framebuffer, 1);
        }
        else if (e->x < 0 && e->x + e->w > 0) {  //左侧超出部分情况
            for (u8 trow = 0; trow < all_row; trow++) {
                for (u8 tcol = 0; tcol < e->w+e->x; tcol++) {
                    framebuffer[trow * WIDTH + tcol] = e->data[trow * e->w -e->x + tcol];
                }
            }
            OLED_GShowChar(32, 6, '-', framebuffer, 1);
        }

        int16_t pr=e->x;
        if (pr < 0) {
            pr = -pr;
            OLED_GShowChar(0, 6, '-', framebuffer, 1);
        }
        OLED_GShowNum(8, 6, pr, 3, framebuffer, 1);
    }
    OLED_RAM_Refresh(framebuffer);
}