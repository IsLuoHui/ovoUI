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

    Menu_Init();

    TEXT te = {8,8,"HelloWorld",OLED_MIX_XOR,0};
    TEXT_Preprocess(&te);

    while (1)
	{
        for (u8 i = 0;i < menu.optnum;i++){
            menu.opt[i].ele.x = GlobalX + (ICON48W + ICONSPACE) * i;
            menu.opt[i].ele.y = GlobalY;
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

        for (u8 i = 0;i < menu.optnum;i++)OLED_Draw_Element(menu.opt[i].ele);

        OLED_Draw_FillRect(cursor.x1, cursor.y1, cursor.x2, cursor.y2, OLED_MIX_XOR);

        OLED_Draw_Point(cursor.x1, cursor.y1, OLED_MIX_XOR);
        OLED_Draw_Point(cursor.x1, cursor.y2-1, OLED_MIX_XOR);
        OLED_Draw_Point(cursor.x2-1, cursor.y1, OLED_MIX_XOR);
        OLED_Draw_Point(cursor.x2-1, cursor.y2-1, OLED_MIX_XOR);

        te.x = cursor.x2;
        te.y = cursor.y1+16;
        OLED_Draw_Text(te);

        //OLED_Draw_Rect(0, 0, cursor.x1, cursor.y1, OLED_MIX_XOR);
        //OLED_Draw_FillRect(0, 56, 128, 64, FrameBuffer, OLED_MIX_XOR);
        //OLED_Draw_Line(127, 63, cursor.x1, cursor.y1, OLED_MIX_XOR);
        //OLED_Draw_DashedLine(0, 32, cursor.x1, cursor.y1, 4, OLED_MIX_XOR);

        OLED_BUFFER_Refresh();

        delay_ms(10);
    }
}
