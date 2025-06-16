#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"

// *OLED 屏幕尺寸
#define OLED_WIDTH 128 //col byte
#define OLED_HEIGHT_PAGE 8   //page

#define OLED_HEIGHT_PIXEL (OLED_HEIGHT_PAGE*8) //pixel
#define OLED_BUFFER_SIZE (OLED_WIDTH * OLED_HEIGHT_PAGE) //byte

// *混合方式
typedef enum {
    OLED_MIX_HIDE   = 0x00,   // 隐藏
    OLED_MIX_COVER  = 0x01,   // 覆盖=
    OLED_MIX_OR     = 0x02,   // 叠加|
    OLED_MIX_AND    = 0x04,   // 叠底&
    OLED_MIX_XOR    = 0x08    // 反色^
} OLED_MIX_MODE;

/**
 *  \brief ELEMENT结构体 用于显示任意矩形元素
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

/**
 *  \brief TEXT结构体 用于显示文本
 */
typedef struct {
    int16_t x, y;
    char *str;
    OLED_MIX_MODE mix; 
    u8 *font[128];
} TEXT;

extern u8 FrameBuffer[OLED_BUFFER_SIZE]; // *OLED显示缓冲区

void OLED_BUFFER_Refresh(void);
void OLED_BUFFER_Clear(void);
void OLED_BUFFER_Fill(void);

void OLED_Draw_Point(u8 x, u8 y, OLED_MIX_MODE mix);
void OLED_Draw_Line(u8 x0, u8 y0, u8 x1, u8 y1, OLED_MIX_MODE mix);
void OLED_Draw_DashedLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 dashlen, OLED_MIX_MODE mix);
void OLED_Draw_Rect(u8 x0, u8 y0, u8 x1, u8 y1, OLED_MIX_MODE mix);
void OLED_Draw_FillRect(u8 x0, u8 y0, u8 x1, u8 y1, OLED_MIX_MODE mix);
void OLED_Draw_Element(ELEMENT ele);

void TEXT_Preprocess(TEXT *text);
void OLED_Draw_Text(TEXT text);

// TODO 重写剩余函数
void OLED_Show_Char(u8 x, u8 page, char c, u8 *RAM, u8 draw);
void OLED_Show_String(u8 x, u8 page, char *str, u8 *RAM, u8 draw);
void OLED_Show_Num(u8 x, u8 page, u32 num, u8 len, u8 *RAM, u8 draw);
void OLED_Show_HexNum(u8 x, u8 page, u32 num, u8 len, u8 *RAM, u8 draw);
void OLED_Show_CN(u8 x, u8 page, u8 CC[], u8 *RAM, u8 draw);
void OLED_Show_CNString(u8 x, u8 page, char *String, u8 *RAM, u8 draw);
void OLED_Show_MixString(u8 x, u8 page, char *String, u8 * RAM, u8 draw);



#endif