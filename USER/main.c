#include "stm32f10x.h"
#include "string.h"
#include "delay.h"
#include "spi.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"
#include "ovoui.h"
#include "tim.h"

int16_t x = 40, y = 8;

extern u8 left;
extern u8 right;
extern int16_t TargetX;

int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    OLED_SPI_GPIO_Init();
    OLED_Init();
    EC11_Init();
    TIM3_Init();

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

        MainMenu[0].ele.x = GlobalX - ICON48W - ICONSPACE;
        MainMenu[1].ele.x = GlobalX;
        MainMenu[2].ele.x = GlobalX + ICON48W + ICONSPACE;

        //for (u8 i = 0;i < elementCount;i++)MainMenu[i].ele.x += x;
        //for (u8 i = 0;i < elementCount;i++)MainMenu[i].ele.y += y;



        memset(FrameBuffer, 0, 1024);

        OLED_Show_Num(0, 0, GlobalX, 5, FrameBuffer, 1);
        //OLED_Show_Num(0, 2, temp, 3, FrameBuffer, 1);
        OLED_Show_Num(0, 4, TargetX, 5, FrameBuffer, 1);
        OLED_Show_HexNum(0, 6, Ec11Trigger, 3, FrameBuffer, 1);
        OLED_Mix_Print();
        
        OLED_RAM_Refresh(FrameBuffer);

        delay_ms(10);
    }
}
