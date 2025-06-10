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
int16_t x = 40, y = 8;

extern ELEMENT *elementPtrs[MAX_ELEMENTS];

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

    //ELEMENT *icon1 = OLED_Element_Create(80, 5, 48, 48, OLED_MIX_COVER,(u8 *)ICON_48X48[0]);

    elementPtrs[0] = OLED_Element_Create(80, 5, 48, 48, OLED_MIX_XOR,(u8 *)ICON_48X48[0]);
    //elementPtrs[0]->x = x;
    //elementPtrs[0]->y = y;
    //elementPtrs[0]->w = 48;
    //elementPtrs[0]->h = 48;
    //elementPtrs[0]->mix = OLED_MIX_AND;
    //lementPtrs[0]->data = (u8*)ICON_48X48[1];

    while (1)
	{
        if (OLEDSHOW)OLED_Display_On();
        else OLED_Display_Off();

        //OLED_Element_Modify(icon1, x, y, 0, 0, (void*)0, OLED_MIX_COVER);
        elementPtrs[0]->x = x;
        elementPtrs[0]->y = y;

        OLED_Mix_Print();
        delay_ms(10);
    }
}
