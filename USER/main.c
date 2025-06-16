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

int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    OLED_SPI_GPIO_Init();
    OLED_Init();
    EC11_Init();
    TIM3_Init();
    Menu_Init();

    while (1)
	{
        for (u8 i = 0;i < menu.optnum;i++){
            menu.opt[i].ele.x = GlobalX + (ICON48W + ICONSPACE) * i;
            menu.opt[i].text.x = GlobalX + (ICON48W + ICONSPACE) * i;
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





        for (u8 i = 0;i < menu.optnum;i++)
        {
            OLED_Draw_Element(menu.opt[i].ele);
            //OLED_Draw_Text(menu.opt[i].text);
        }
        OLED_Draw_FillRect(cursor.x0, cursor.y0, cursor.x1, cursor.y1, OLED_MIX_XOR);

        extern u8 tT;
        if (tT & 0x08)OLED_Show_Char(0, 6, '1', FrameBuffer, 1);
		if (tT & 0x04)OLED_Show_Char(8, 6, '1', FrameBuffer, 1);
		if (tT & 0x02)OLED_Show_Char(16, 6, '1', FrameBuffer, 1);
		if (tT & 0x01)OLED_Show_Char(24, 6, '1', FrameBuffer, 1);


        OLED_BUFFER_Refresh();

        delay_ms(10);
    }
}
