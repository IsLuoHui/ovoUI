#include "ovoui.h"
#include "font.h"
#include "stdio.h"

MENU menus[1];
u8 option_num;

void Setting(void) {
    
}

OPTION MainMenu[] = {
    { {0, 0, 0, 0, OLED_MIX_HIDE, (void*)0}, 0 },
    { {0, 0, 0, 0, OLED_MIX_HIDE, (void*)0}, 0 },
    { {0, 0, 0, 0, OLED_MIX_HIDE, (void*)0}, 0 },
    { {0, 0, 0, 0, OLED_MIX_HIDE, (void*)0}, 0 },
};

void MainMenuInit(void) {
    menus[0].opt = MainMenu;
    menus[0].mode = MENU_SHOW_Horizontal;
    menus[0].optnum = sizeof(MainMenu) / sizeof(MainMenu[0]);
    menus[0].position = 40;
}
