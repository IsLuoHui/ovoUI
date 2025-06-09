#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"


//时钟 D1
#define OLED_SPI_CLK_GPIO GPIOA
#define OLED_SPI_CLK_PIN GPIO_Pin_5
#define OLED_SPI_CLK_PIN_ID 5
#define OLED_SPI_CLK_PIN_RCC RCC_APB2Periph_GPIOA
//数据 D1
#define OLED_SPI_DIN_GPIO GPIOA
#define OLED_SPI_DIN_PIN GPIO_Pin_6
#define OLED_SPI_DIN_PIN_ID 6
#define OLED_SPI_DIN_PIN_RCC RCC_APB2Periph_GPIOA
//重置 RES
#define OLED_SPI_RES_GPIO GPIOB
#define OLED_SPI_RES_PIN GPIO_Pin_0
#define OLED_SPI_RES_PIN_ID 0
#define OLED_SPI_RES_PIN_RCC RCC_APB2Periph_GPIOB
//命令 DC
#define OLED_SPI_DC_GPIO GPIOB
#define OLED_SPI_DC_PIN GPIO_Pin_1
#define OLED_SPI_DC_PIN_ID 1
#define OLED_SPI_DC_PIN_RCC RCC_APB2Periph_GPIOB
//片选 CD
#define OLED_SPI_CS_GPIO GPIOA
#define OLED_SPI_CS_PIN GPIO_Pin_4
#define OLED_SPI_CS_PIN_ID 4
#define OLED_SPI_CS_PIN_RCC RCC_APB2Periph_GPIOA
//宽高 W & H
#define OLED_WIDTH 128  //byte
#define OLED_HEIGHT 8   //page

#define OLED_SIZE (OLED_WIDTH * OLED_HEIGHT)

#define OLED_SPI_CLK_H (OLED_SPI_CLK_GPIO->BSRR = OLED_SPI_CLK_PIN)
#define OLED_SPI_CLK_L (OLED_SPI_CLK_GPIO->BRR = OLED_SPI_CLK_PIN)
#define OLED_SPI_DIN_H (OLED_SPI_DIN_GPIO->BSRR = OLED_SPI_DIN_PIN)
#define OLED_SPI_DIN_L (OLED_SPI_DIN_GPIO->BRR = OLED_SPI_DIN_PIN)
#define OLED_SPI_RES_H (OLED_SPI_RES_GPIO->BSRR = OLED_SPI_RES_PIN)
#define OLED_SPI_RES_L (OLED_SPI_RES_GPIO->BRR = OLED_SPI_RES_PIN)
#define OLED_SPI_DC_H (OLED_SPI_DC_GPIO->BSRR = OLED_SPI_DC_PIN)
#define OLED_SPI_DC_L (OLED_SPI_DC_GPIO->BRR = OLED_SPI_DC_PIN)
#define OLED_SPI_CS_H (OLED_SPI_CS_GPIO->BSRR = OLED_SPI_CS_PIN)
#define OLED_SPI_CS_L (OLED_SPI_CS_GPIO->BRR = OLED_SPI_CS_PIN)


#define SIZE 16
#define Max_Column	128
#define Max_Row		64

void OLED_SPI_GPIO_Init(void);
void OLED_Init(void);
void OLED_W_CMD(u8 cmd);
void OLED_W_DATA(u8 data);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Cursor(u8 x, u8 y);

void OLED_Screen_Clear(void);
void OLED_Screen_Fill(void);
//RAM
void OLED_RAM_Refresh(u8 *RAM);
void OLED_RAM_Clear(u8 *RAM);
void OLED_Fill_RAM(u8 *RAM);
void OLED_DrawPoint(u8 x, u8 y, u8 *RAM, u8 draw);
void OLED_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw);
void OLED_DrawDashedLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 dashlen, u8 *RAM, u8 draw);
void OLED_DrawRect(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw);
void OLED_DrawFillRect(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw);

void OLED_GShowChar(u8 x, u8 y, char c, u8 *RAM, u8 draw);
void OLED_GShowString(u8 x, u8 y, char *str, u8 *RAM, u8 draw);
void OLED_GShowNum(u8 x, u8 y, u32 num, u8 len, u8 *RAM, u8 draw);
void OLED_GShowHexNum(u8 x, u8 y, u32 num, u8 len, u8 *RAM, u8 draw);
void OLED_GShowCN(u8 x, u8 y, u8 CC[], u8 *RAM, u8 draw);
void OLED_GShowCNString(u8 x, u8 y, char *String, u8 *RAM, u8 draw);
void OLED_GShowMixString(u8 x, u8 y, char *String, u8 * RAM, u8 draw);

void OLED_GShowBMP(u8 x, u8 y, u8 w, u8 h, u8 BMP[], u8 *RAM, u8 draw);

#endif
