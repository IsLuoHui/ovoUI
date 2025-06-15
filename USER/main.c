#include "stm32f10x.h"
#include "string.h"
#include "delay.h"
#include "spi.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"
#include "ovoui.h"
#include "tim.h"

//#define DEBUG



extern float xt;
extern float yt;

extern u8 keydown;

int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    OLED_SPI_GPIO_Init();
    OLED_Init();
    EC11_Init();
    TIM3_Init();

    _Main();

    while (1)
	{
        for (u8 i = 0;i < menus[screen].optnum;i++){
            menus[screen].opt[i].ele.x = GlobalX + (ICON48W + ICONSPACE) * i;
            menus[screen].opt[i].ele.y = GlobalY;
        }




        memset(FrameBuffer, 0, 1024);

        #ifdef DEBUG
        int16_t gx = GlobalX;
        if (gx < 0) {
            gx = -gx;
            OLED_Show_Char(0, 0, '-', FrameBuffer, 1);
        }
        OLED_Show_Num(8, 0, gx, 5, FrameBuffer, 1);
        OLED_Show_Num(0, 2, keydown, 1, FrameBuffer, 1);
        OLED_Show_Num(0, 4, (u16)(yt * 1000), 4, FrameBuffer, 1);
        OLED_Show_Num(0, 6, (u16)(xt * 1000), 4, FrameBuffer, 1);
        #endif

        for (u8 i = 0;i < menus[screen].optnum;i++)OLED_Show_Element(menus[screen].opt[i].ele);

        OLED_Draw_FillRect(cursor.x1, cursor.y1, cursor.x2, cursor.y2, FrameBuffer, OLED_MIX_XOR);
        //OLED_Draw_Point(cursor.x1, cursor.y1, FrameBuffer, 0);
        //OLED_Draw_Point(cursor.x1, cursor.y2-1, FrameBuffer, 0);
        //OLED_Draw_Point(cursor.x2-1, cursor.y1, FrameBuffer, 0);
        //OLED_Draw_Point(cursor.x2-1, cursor.y2-1, FrameBuffer, 0);

        //OLED_Draw_FillRect(0, 0, 128, 8, FrameBuffer, OLED_MIX_XOR);
        //OLED_Draw_FillRect(0, 56, 128, 64, FrameBuffer, OLED_MIX_XOR);
        //OLED_Show_String(0, 0, "OLED_Demo", FrameBuffer, 0);

        OLED_RAM_Refresh(FrameBuffer);

        delay_ms(10);
    }
}
