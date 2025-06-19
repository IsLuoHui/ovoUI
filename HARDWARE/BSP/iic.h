#ifndef __IIC_H
#define __IIC_H
#include "stm32f10x.h"
#include "delay.h"

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

#define DL 1 // IIC时序延时
#define OLED_ADDR 0x78 // IIC地址

static inline void OLED_IIC_Start(void)
{
    OLED_IIC_SDA_H;
    delay_us(DL);
    OLED_IIC_SCL_H;
    delay_us(DL);
    OLED_IIC_SDA_L;
    delay_us(DL);
    OLED_IIC_SCL_L;
    delay_us(DL);
}

static inline void OLED_IIC_Stop(void)
{
	OLED_IIC_SDA_L;
    delay_us(DL);
	OLED_IIC_SCL_H;
    delay_us(DL);
	OLED_IIC_SDA_H;
    delay_us(DL);
}

static inline void OLED_IIC_SendByte(u8 byte)
{
	u8 i;
	for (i = 0; i < 8; i++)
	{
		if(byte & 0x80)OLED_IIC_SDA_H;
		else OLED_IIC_SDA_L;
        delay_us(DL);
		OLED_IIC_SCL_H;
        delay_us(DL);
		OLED_IIC_SCL_L;
        delay_us(DL);
		byte<<=1;
	}
	
	OLED_IIC_SCL_H;
    delay_us(DL);
	OLED_IIC_SCL_L;
    delay_us(DL);
}

void OLED_IIC_GPIO_Init(void);
void OLED_IIC_Init(void);
void OLED_IIC_W_CMD(u8 cmd);
void OLED_IIC_W_DATA(u8 data);
void OLED_IIC_Set_Cursor(u8 x, u8 page);

#endif
