#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"

// *OLED 屏幕尺寸
#define OLED_WIDTH 128 //col byte
#define OLED_HEIGHT_PAGE 8   //page

#define OLED_HEIGHT_PIXEL (OLED_HEIGHT_PAGE*8) //page
#define OLED_BUFFER_SIZE (OLED_WIDTH * OLED_HEIGHT_PAGE)

#define MAX_ELEMENTS 20 // 最大元素数量

//元素混合方式
typedef enum {
    OLED_MIX_HIDE   = 0x00,   // 隐藏
    OLED_MIX_COVER  = 0x01,   // 覆盖
    OLED_MIX_OR     = 0x02,   // 叠加
    OLED_MIX_AND    = 0x04,   // 叠底
    OLED_MIX_XOR    = 0x08    // 反色
} OLED_MIX_MODE;

/**
 *  *ELEMENT结构体 屏幕左上角为坐标原点(0,0),其中x,y,w,h单位均为像素
 *  \param x 左向右X正方向
 *  \param y 上向下Y正方向
 *  \param w 矩形宽度
 *  \param h 矩形高度
 *  \param mix 混合模式
 *  \param *data 数据指针
 *  \warning 请确保`*data`指向数组能转化为矩形，且转化后所有页相加大于`h`，即data_len/w*8>=h
 *
 */
typedef struct {
    int16_t x, y;
    u8 w, h;
    OLED_MIX_MODE mix; 
    u8 *data;
} ELEMENT;

void OLED_RAM_Refresh(u8 *RAM);
void OLED_RAM_Clear(u8 *RAM);
void OLED_RAM_Fill(u8 *RAM);

void OLED_Draw_Point(u8 x, u8 y, u8 *RAM, u8 draw);
void OLED_Draw_Line(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw);
void OLED_Draw_DashedLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 dashlen, u8 *RAM, u8 draw);
void OLED_Draw_Rect(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw);
void OLED_Draw_FillRect(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw);

void OLED_Show_Char(u8 x, u8 page, char c, u8 *RAM, u8 draw);
void OLED_Show_String(u8 x, u8 page, char *str, u8 *RAM, u8 draw);
void OLED_Show_Num(u8 x, u8 page, u32 num, u8 len, u8 *RAM, u8 draw);
void OLED_Show_HexNum(u8 x, u8 page, u32 num, u8 len, u8 *RAM, u8 draw);
void OLED_Show_CN(u8 x, u8 page, u8 CC[], u8 *RAM, u8 draw);
void OLED_Show_CNString(u8 x, u8 page, char *String, u8 *RAM, u8 draw);
void OLED_Show_BMP(u8 x, u8 page, u8 w, u8 h, u8 BMP[], u8 *RAM, u8 draw);
void OLED_Show_MixString(u8 x, u8 page, char *String, u8 * RAM, u8 draw);

ELEMENT *OLED_Element_Create(int16_t x, int16_t y, u8 w, u8 h, OLED_MIX_MODE mix, u8 *data);
void OLED_Element_Modify(ELEMENT *ele, int16_t x, int16_t y, u8 w, u8 h, OLED_MIX_MODE mix, u8 *data);
void OLED_Element_Remove(u8 index);

void OLED_Mix_Print();

extern u8 FrameBuffer[OLED_BUFFER_SIZE]; // *OLED显示缓冲区
extern ELEMENT *elementPtrs[MAX_ELEMENTS]; // *动态分配ELEMENT数组
extern u8 elementCount; // *当前元素数量

#endif