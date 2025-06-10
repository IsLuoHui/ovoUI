#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"

// **定义 OLED 屏幕尺寸**
#define WIDTH 128
#define HEIGHT 64
#define MAX_ELEMENTS 20 // 最大元素数量

// **定义 ELEMENT 结构体**
typedef struct {
    int16_t x, y;   // 元素位置（像素单位）
    u8 w, h;   // 元素宽度、高度（像素单位）
    u8 mix;    // 显示模式 0x00:覆盖 0x01:叠加 0x02:反色
    u8 *data;  // 显示数据指针
} ELEMENT;

ELEMENT *OLED_Create_Element(int16_t x, int16_t y, u8 w, u8 h,u8 mix, u8 *data);
void modifyElement(ELEMENT *ele, int16_t x, int16_t y, u8 w, u8 h, u8 *data, u8 mix);
void removeElement(u8 index);


void OLED_Mix_Print();

#endif