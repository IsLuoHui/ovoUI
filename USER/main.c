#include "stm32f10x.h"
#include "string.h"
#include "delay.h"
#include "spi.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"
#include "ovoui.h"
#include "tim.h"

#define DEBUG

extern int16_t TargetX;

extern u8 cur_x1;
extern u8 cur_y1;
extern u8 cur_x2;
extern u8 cur_y2;

extern float t;

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
        }




        memset(FrameBuffer, 0, 1024);

        #ifdef DEBUG
        int16_t gx = GlobalX;
        if (gx < 0) {
            gx = -gx;
            OLED_Show_Char(0, 0, '-', FrameBuffer, 1);
        }
        OLED_Show_Num(8, 0, gx, 5, FrameBuffer, 1);
        OLED_Show_Num(0, 4, keydown, 1, FrameBuffer, 1);
        OLED_Show_Num(0, 6, (u16)(t * 1000), 4, FrameBuffer, 1);
        #endif

        for (u8 i = 0;i < menus[screen].optnum;i++)OLED_Show_Element(menus[screen].opt[i].ele);

        OLED_Draw_FillRect(cur_x1, cur_y1, cur_x2, cur_y2, FrameBuffer, OLED_MIX_XOR);
        OLED_Draw_Point(cur_x1, cur_y1, FrameBuffer, 2);
        OLED_Draw_Point(cur_x1, cur_y2-1, FrameBuffer, 2);
        OLED_Draw_Point(cur_x2-1, cur_y1, FrameBuffer, 2);
        OLED_Draw_Point(cur_x2-1, cur_y2-1, FrameBuffer, 2);

        OLED_RAM_Refresh(FrameBuffer);

        delay_ms(10);
    }
}
