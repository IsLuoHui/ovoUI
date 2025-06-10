#include "stm32f10x.h"
#include "delay.h"
#include "spi.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"


#define LED_OFF (GPIOC->BSRR = GPIO_Pin_13) //H
#define LED_ON (GPIOC->BRR = GPIO_Pin_13)	//L

u8 ec11 = 0x00;
extern u8 OLEDSHOW;
int16_t x = 40, y = 0;

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

    //u8 ic[] = {
	//	0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff
	//};
	//ELEMENT *icon1 = OLED_Create_Element(110, 5, 8, 8, 0x00,ic);
	ELEMENT *icon1 = OLED_Create_Element(80, 5, 48, 48, 0x00,(u8 *)ICON_48X48[1]);
	while (1)
	{
        if (OLEDSHOW)OLED_Display_On();
        else OLED_Display_Off();

        modifyElement(icon1, x, y, 0, 0, (void*)0, 0);
        OLED_Mix_Print();
        delay_ms(10);
    }
}
