#include "stm32f10x.h"
#include "delay.h"
#include "string.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"
#include "ovoui.h"
#include "tim.h"

//#define DEBUGD

int main() {
    OLED_Init();


    EC11_Init();
    TIM3_Init();
    Menu_Init();

    while (1)
    {

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
            OLED_Draw_Text(menu.opt[i].text);
            for (u8 j = 0;j < menu.opt[i].listnum;j++)
                OLED_Draw_Text(menu.opt[i].list[j].text);
        }
        OLED_Draw_FillRect(cursor.x0, cursor.y0, cursor.x1, cursor.y1, OLED_MIX_XOR);
        OLED_Draw_Point(cursor.x0, cursor.y0, OLED_MIX_XOR);
        OLED_Draw_Point(cursor.x0, cursor.y1 - 1, OLED_MIX_XOR);
        OLED_Draw_Point(cursor.x1 - 1, cursor.y0, OLED_MIX_XOR);
        OLED_Draw_Point(cursor.x1 - 1, cursor.y1 - 1, OLED_MIX_XOR);

        //滚动条背景
        OLED_Draw_Rect(121 + scrollbarOffset, 0, OLED_WIDTH - 1 + scrollbarOffset, OLED_HEIGHT_PIXEL - 1, OLED_MIX_COVER);
        //滚动条滑块
        OLED_Draw_FillRect(122 + scrollbarOffset, 0, OLED_WIDTH - 1 + scrollbarOffset, scrollOffset, OLED_MIX_COVER);


        //OLED_Show_Num(0, 6, MENUCHOICE, 1, FrameBuffer, 1);
        //OLED_Show_Num(16, 6, OPTIONCHOICE, 1, FrameBuffer, 1);

        OLED_Buffer_Refresh();

        //delay_ms(10);
    }
}
