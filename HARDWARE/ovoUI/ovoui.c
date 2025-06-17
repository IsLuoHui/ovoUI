#include "ovoui.h"
#include "font.h"
#include "tim.h"
#include "string.h"

LIST list1[] = {
    {{0, 0, "c1", OLED_MIX_XOR, 0},0},
    {{0, 0, "c2", OLED_MIX_XOR, 0},0},
    {{0, 0, "c3", OLED_MIX_XOR, 0},0},
    {{0, 0, "c4", OLED_MIX_XOR, 0},0},
};

OPTION MainMenu[] = {
    {
        {0,0,"Port",OLED_MIX_HIDE,0},
        {0, 0, 0, 0, OLED_MIX_HIDE,(u8 *)ICON_48X48[0]},
        list1,
        sizeof(list1) / sizeof(list1[0])
    },
    {
        {0,0,"Model",OLED_MIX_HIDE,0},
        {0, 0, 0, 0, OLED_MIX_HIDE,(u8 *)ICON_48X48[1]},
        list1,
        sizeof(list1) / sizeof(list1[0])
    },
    {
        {0,0,"Setting",OLED_MIX_HIDE,0},
        {0, 0, 0, 0, OLED_MIX_HIDE, (u8 *)ICON_48X48[2]},
        list1,
        sizeof(list1) / sizeof(list1[0])
    },
    {
        {0,0,"Info",OLED_MIX_HIDE,0},
        {0, 0, 0, 0, OLED_MIX_HIDE, (u8 *)ICON_48X48[3]},
        list1,
        sizeof(list1) / sizeof(list1[0])
    },
};

MENU menu = {MainMenu,sizeof(MainMenu) / sizeof(MainMenu[0]),MENULEFTEND,0};

CURSOR cursor = {40, 0, 40 + ICON48W, ICON48H};

int16_t menuOffsetX = 0;
int16_t menuOffsetY = 0;

// TODO 补充初始化
void Menu_Init(void) {
    for (u8 i = 0; i < menu.optnum; i++) {


        menu.opt[i].text.x = (OLED_WIDTH - strlen(menu.opt[i].text.str) * 8) / 2;
        menu.opt[i].text.y = 64;
        menu.opt[i].text.mix = OLED_MIX_OR;
        TEXT_Preprocess(&menu.opt[i].text);

        menu.opt[i].ele.x = MENULEFTEND + (ICON48W + ICONSPACE) * i;
        menu.opt[i].ele.y = 0;
        menu.opt[i].ele.w = ICON48W;
        menu.opt[i].ele.h = ICON48H;
        menu.opt[i].ele.mix = OLED_MIX_XOR;
        //menu.opt[i].ele.data = (u8 *)ICON_48X48[i];
    }
    //menuOffsetX = menu.offset;
    Position_Init(menu);
}

void On_Menu_Prev(void) {
    menu.offset += (ICONSPACE + ICON48W);
}

void On_Menu_Next(void) {
    menu.offset -= (ICONSPACE + ICON48W);
}

void On_Menu_Enter(void) {

}