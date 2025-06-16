#include "ovoui.h"
#include "font.h"
#include "stdio.h"
#include "tim.h"

u8 screen = 0;

extern float at;

// TODO 补充初始化
void Menu_Init(void) {
    for (u8 i = 0; i < menu.optnum; i++) {
        menu.opt[i].ele.y = 0;
        menu.opt[i].ele.w = ICON48W;
        menu.opt[i].ele.h = ICON48H;
        menu.opt[i].ele.mix = OLED_MIX_XOR;
        //menu.opt[i].ele.data = (u8 *)ICON_48X48[i];
    }
    Position_Init(menu);
}

LIST list1[] = {
    {{0, 0, "c1", OLED_MIX_XOR, 0},0},
    {{0, 0, "c2", OLED_MIX_XOR, 0},0},
    {{0, 0, "c3", OLED_MIX_XOR, 0},0},
    {{0, 0, "c4", OLED_MIX_XOR, 0},0},
};

OPTION MainMenu[] = {
    {
        {0,0,"1",OLED_MIX_HIDE,0},
        {0, 0, 0, 0, OLED_MIX_HIDE,(u8 *)ICON_48X48[1]},
        list1,
        sizeof(list1) / sizeof(list1[0])
    },
    {
        {0,0,"2",OLED_MIX_HIDE,0},
        {0, 0, 0, 0, OLED_MIX_HIDE, (u8 *)ICON_48X48[2]},
        list1,
        sizeof(list1) / sizeof(list1[0])
    },
    {
        {0,0,"3",OLED_MIX_HIDE,0},
        {0, 0, 0, 0, OLED_MIX_HIDE, (u8 *)ICON_48X48[3]},
        list1,
        sizeof(list1) / sizeof(list1[0])
    },
};


MENU menu = {MainMenu,sizeof(MainMenu) / sizeof(MainMenu[0]),40,40};