#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"

// *OLED 屏幕尺寸(Pixel)
#define WIDTH 128
#define HEIGHT 64
#define MAX_ELEMENTS 20 // 最大元素数量

// *ELEMENT 结构体
typedef struct {
    int16_t x, y;
    u8 w, h;
    u8 mix;
    u8 *data;
} ELEMENT;

//RAM
void OLED_RAM_Refresh(u8 *RAM);
void OLED_RAM_Clear(u8 *RAM);
void OLED_Fill_RAM(u8 *RAM);
void OLED_DrawPoint(u8 x, u8 y, u8 *RAM, u8 draw);
void OLED_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw);
void OLED_DrawDashedLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 dashlen, u8 *RAM, u8 draw);
void OLED_DrawRect(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw);
void OLED_DrawFillRect(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw);

void OLED_ShowChar(u8 x, u8 page, char c, u8 *RAM, u8 draw);
void OLED_ShowString(u8 x, u8 page, char *str, u8 *RAM, u8 draw);
void OLED_ShowNum(u8 x, u8 page, u32 num, u8 len, u8 *RAM, u8 draw);
void OLED_ShowHexNum(u8 x, u8 page, u32 num, u8 len, u8 *RAM, u8 draw);
void OLED_ShowCN(u8 x, u8 page, u8 CC[], u8 *RAM, u8 draw);
void OLED_ShowCNString(u8 x, u8 page, char *String, u8 *RAM, u8 draw);
void OLED_ShowBMP(u8 x, u8 page, u8 w, u8 h, u8 BMP[], u8 *RAM, u8 draw);
void OLED_ShowMixString(u8 x, u8 page, char *String, u8 * RAM, u8 draw);

//宽高 W & H
#define OLED_WIDTH 128  //byte
#define OLED_HEIGHT 8   //page

#define OLED_SIZE (OLED_WIDTH * OLED_HEIGHT)


ELEMENT *OLED_Create_Element(int16_t x, int16_t y, u8 w, u8 h, u8 mix, u8 *data);
void modifyElement(ELEMENT *ele, int16_t x, int16_t y, u8 w, u8 h, u8 *data, u8 mix);
void removeElement(u8 index);


void OLED_Mix_Print();

#endif