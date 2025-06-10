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

        // 边界保护
        if (e->x >= WIDTH || e->y >= HEIGHT || e->x + e->w <= 0 || e->y + e->h <= 0) continue;

        int16_t x0 = e->x < 0 ? 0 : e->x;
        int16_t x1 = (e->x + e->w > WIDTH) ? WIDTH : (e->x + e->w);

        int16_t page_start;
        u8 y_offset;
        if (e->y >= 0) {
            page_start = e->y / 8;
            y_offset = e->y % 8;
        } else {
            int16_t abs_y = -e->y;
            page_start = (e->y - 7) / 8; // 向下取整
            y_offset = (8 - (abs_y % 8)) % 8;
        }

        // 计算需要显示的页数，确保像素全部显示
        u8 page_cnt = (e->h + y_offset + 7) / 8;

        for (int16_t col = x0; col < x1; col++) {
            int16_t data_col = col - e->x;
            u8 prev = 0;
            for (u8 page = 0; page < page_cnt; page++) {
                u8 data = 0;
                int16_t data_idx = page * e->w + data_col;
                if (data_col >= 0 && data_col < e->w && data_idx < e->w * ((e->h + 7) / 8))
                    data = e->data[data_idx];

                u8 out;
                if (page == 0) {
                    out = data << y_offset;
                } else {
                    out = (data << y_offset) | (prev >> (8 - y_offset));
                }
                prev = data;

                int16_t fb_page = page_start + page;
                if (col >= 0 && col < WIDTH && fb_page >= 0 && fb_page < HEIGHT / 8)
                    framebuffer[fb_page * WIDTH + col] = out;
            }
        }

        int16_t px = e->x;
        int16_t py = e->y;
        if (px < 0) {
            px = -px;
            OLED_GShowChar(0, 6, '-', framebuffer, 1);
        }
        if (py < 0) {
            py = -py;
            OLED_GShowChar(0, 4, '-', framebuffer, 1);
        }
        OLED_GShowNum(8, 4, py, 3, framebuffer, 1);
        OLED_GShowNum(8, 6, px, 3, framebuffer, 1);
    }
    OLED_RAM_Refresh(framebuffer);
}
 /*
        int16_t px = e->x;
        int16_t py = e->y;
        if (px < 0) {
            px = -px;
            OLED_GShowChar(0, 6, '-', framebuffer, 1);
        }
        if (py < 0) {
            py = -py;
            OLED_GShowChar(0, 4, '-', framebuffer, 1);
        }
        OLED_GShowNum(8, 4, py, 3, framebuffer, 1);
        OLED_GShowNum(8, 6, px, 3, framebuffer, 1);
        */