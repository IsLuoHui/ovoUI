#include "ovoui.h"
#include "font.h"
#include "stdio.h"

void Setting(void) {
    
}

OPTION MainMenu[] = {
    { {0, 0, 0, 0, OLED_MIX_HIDE, (void*)0}, 0 },
    { {0, 0, 0, 0, OLED_MIX_HIDE, (void*)0}, 0 },
    { {0, 0, 0, 0, OLED_MIX_HIDE, (void*)0}, 0 },
    { {0, 0, 0, 0, OLED_MIX_HIDE, (void*)0}, 0 },
};

MENU menus[] = {
    {MainMenu,MENU_SHOW_Horizontal,sizeof(MainMenu) / sizeof(MainMenu[0]),40,-32}
};