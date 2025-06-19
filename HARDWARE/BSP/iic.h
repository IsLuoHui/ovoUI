#ifndef __IIC_H
#define __IIC_H
#include "stm32f10x.h"

// 时钟 SCL
#define OLED_IIC_SCL_GPIO GPIOB
#define OLED_IIC_SCL_PIN GPIO_Pin_12
#define OLED_IIC_SCL_PIN_ID 12
#define OLED_IIC_SCL_PIN_RCC RCC_APB2Periph_GPIOB

// 数据 SDA
#define OLED_IIC_SDA_GPIO GPIOB
#define OLED_IIC_SDA_PIN GPIO_Pin_13
#define OLED_IIC_SDA_PIN_ID 13
#define OLED_IIC_SDA_PIN_RCC RCC_APB2Periph_GPIOB

#define OLED_IIC_SCL_H (OLED_IIC_SCL_GPIO->BSRR =   OLED_IIC_SCL_PIN)
#define OLED_IIC_SCL_L (OLED_IIC_SCL_GPIO->BRR =    OLED_IIC_SCL_PIN)
#define OLED_IIC_SDA_H (OLED_IIC_SDA_GPIO->BSRR =   OLED_IIC_SDA_PIN)
#define OLED_IIC_SDA_L (OLED_IIC_SDA_GPIO->BRR =    OLED_IIC_SDA_PIN)

void OLED_IIC_GPIO_Init(void);
void OLED_IIC_Init(void);
void OLED_IIC_W_CMD(u8 cmd);
void OLED_IIC_W_DATA(u8 data);

void OLED_SetCursor(u8 x, u8 y);
void OLED_CLS(void);

#endif
