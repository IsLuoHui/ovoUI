#include "stm32f10x.h"
#include "delay.h"
#include "spi.h"
#include "ec11.h"

#define LED_OFF (GPIOC->BSRR = GPIO_Pin_13) //H
#define LED_ON (GPIOC->BRR = GPIO_Pin_13)	//L

//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PA5（SCL）
//              D1   接PA7（SDA）
//              RES  接PB0
//              DC   接PB1
//              CS   接PA4 

u8 OLEDRAM[OLED_SIZE];
u8 ec11 = 0x00;
//u8 r = 0;

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	LED_OFF;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_SPI_GPIO_Init();
	OLED_Init();
	EC11_Init();

	while (1)
	{
		// if (ec11 & 0x05)r++;
		// if (ec11 & 0x03)r--;
		// if (ec11 & 0x04 && !(ec11&0x01))r += 10;
		// if (ec11 & 0x02 && !(ec11&0x01))r -= 10;

		//OLED_GShowString(0, 2, "00000000", OLEDRAM, 1);
		if (ec11 & 0x80)OLED_GShowChar(0, 2, '1', OLEDRAM, 1);
		if (ec11 & 0x40)OLED_GShowChar(8, 2, '1', OLEDRAM, 1);
		if (ec11 & 0x20)OLED_GShowChar(16, 2, '1', OLEDRAM, 1);
		if (ec11 & 0x10)OLED_GShowChar(24, 2, '1', OLEDRAM, 1);
		if (ec11 & 0x08)OLED_GShowChar(32, 2, '1', OLEDRAM, 1);
		if (ec11 & 0x04)OLED_GShowChar(40, 2, '1', OLEDRAM, 1);
		if (ec11 & 0x02)OLED_GShowChar(48, 2, '1', OLEDRAM, 1);
		if (ec11 & 0x01)OLED_GShowChar(56, 2, '1', OLEDRAM, 1);
		//OLED_GShowNum(0, 2, kv, 8, OLEDRAM, 1);
		//OLED_GShowNum(0, 4, k, 3, OLEDRAM, 1);
		//OLED_GShowNum(0, 6, r, 3, OLEDRAM, 1);
		OLED_Refresh_RAM(OLEDRAM);
		delay_ms(10);
		OLED_Clear_RAM(OLEDRAM);
	}
	
}
