#include "stm32f10x.h"
#include "string.h"
#include "delay.h"
#include "spi.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"
#include "ovoui.h"


#define LED_OFF (GPIOC->BSRR = GPIO_Pin_13) //H
#define LED_ON (GPIOC->BRR = GPIO_Pin_13)	//L

int16_t x = 40, y = 8;

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

    MenuInit();
    for (u8 i = 0;i < elementCount;i++)MainMenu[i].ele.data = (u8 *)ICON_48X48[i];
    for (u8 i = 0;i < elementCount;i++)MainMenu[i].ele.y = y;
    //MainMenu[0].ele.data = (u8 *)ICON_48X48[1];

    //elementPtrs[0]->x = x;
    //elementPtrs[0]->y = y;
    //elementPtrs[0]->w = 48;
    //elementPtrs[0]->h = 48;
    //elementPtrs[0]->mix = OLED_MIX_AND;

    while (1)
	{

        //OLED_Element_Modify(icon1, x, y, 0, 0, (void*)0, OLED_MIX_COVER);
        //elementPtrs[0]->x = x;
        //elementPtrs[0]->y = y;

        MainMenu[0].ele.x = x - ICON48W - ICONSPACE;
        MainMenu[1].ele.x = x;
        MainMenu[2].ele.x = x + ICON48W + ICONSPACE;

        //for (u8 i = 0;i < elementCount;i++)MainMenu[i].ele.x += x;
        //for (u8 i = 0;i < elementCount;i++)MainMenu[i].ele.y += y;


        memset(FrameBuffer, 0, 1024);
        OLED_Mix_Print();
        OLED_RAM_Refresh(FrameBuffer);

        delay_ms(10);
    }
}
