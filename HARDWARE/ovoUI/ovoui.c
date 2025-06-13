#include "ovoui.h"
#include "font.h"
#include "stdio.h"
#include "tim.h"

u8 screen = 0;


// TODO 记忆选项
void _Main_IN(void) {
    for (u8 i = 0;i < menus[0].optnum;i++) {
        menus[0].opt[i].ele.y = 8;
        menus[0].opt[i].ele.w = ICON48W;
        menus[0].opt[i].ele.h = ICON48H;
        menus[0].opt[i].ele.mix = OLED_MIX_XOR;
        //menus[0].opt[i].ele.data = (u8 *)ICON_48X48[i];
    }
    screen = 0;
    Position_Init(menus[0]);
}

void _Model_IN(void) {
    for (u8 i = 0;i < menus[1].optnum;i++) {
        menus[1].opt[i].ele.y = 8;
        menus[1].opt[i].ele.w = ICON48W;
        menus[1].opt[i].ele.h = ICON48H;
        menus[1].opt[i].ele.mix = OLED_MIX_XOR;
        //menus[0].opt[i].ele.data = (u8 *)ICON_48X48[i];
    }
    screen = 1;
    Position_Init(menus[1]);
}

OPTION MainMenu[] = {
    { {0, 0, 0, 0, OLED_MIX_HIDE, (u8 *)ICON_48X48[1]}, 0 },
    { {0, 0, 0, 0, OLED_MIX_HIDE, (u8 *)ICON_48X48[2]}, 0 },
    { {0, 0, 0, 0, OLED_MIX_HIDE, (u8 *)ICON_48X48[3]}, _Model_IN },
};

OPTION ModelMenu[] = {
    { {0, 0, 0, 0, OLED_MIX_HIDE, (u8 *)ICON_48X48[0]}, 0 },
    { {0, 0, 0, 0, OLED_MIX_HIDE, (u8 *)ICON_48X48[4]}, _Main_IN },
};


MENU menus[] = {
    {MainMenu,MENU_SHOW_Horizontal,sizeof(MainMenu) / sizeof(MainMenu[0]),40,40},
    {ModelMenu,MENU_SHOW_Horizontal,sizeof(ModelMenu) / sizeof(ModelMenu[0]),40,40},
};