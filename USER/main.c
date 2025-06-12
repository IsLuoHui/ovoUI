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

extern u8 cur_x;
extern u8 cur_y;
extern u8 cur_w;
extern u8 cur_h;

int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    OLED_SPI_GPIO_Init();
    OLED_Init();
    EC11_Init();
    TIM3_Init();


    MainMenuInit();


    for (u8 i = 0;i < menus[0].optnum;i++) {
        menus[0].opt[i].ele.y = y;
        menus[0].opt[i].ele.w = ICON48W;
        menus[0].opt[i].ele.h = ICON48H;
        menus[0].opt[i].ele.mix = OLED_MIX_XOR;
        menus[0].opt[i].ele.data = (u8 *)ICON_48X48[i];
    }
    Position_Init(menus[0]);

    while (1)
	{
        for (u8 i = 0;i < menus[0].optnum;i++){
            menus[0].opt[i].ele.x = GlobalX + (ICON48W + ICONSPACE) * i;
        }




        memset(FrameBuffer, 0, 1024);

        OLED_Show_Num(0, 0, GlobalX, 5, FrameBuffer, 1);
        //OLED_Show_Num(0, 2, option_num, 2, FrameBuffer, 1);
        //OLED_Show_HexNum(0, 4, Ec11Trigger, 3, FrameBuffer, 1);
        OLED_Show_Num(0, 6, TargetX, 5, FrameBuffer, 1);

        for (u8 i = 0;i < menus[0].optnum;i++)OLED_Show_Element(menus[0].opt[i].ele);

        OLED_Draw_FillRect(cur_x, cur_y, cur_w, cur_h, FrameBuffer, OLED_MIX_XOR);

        OLED_RAM_Refresh(FrameBuffer);

        delay_ms(10);
    }
}
