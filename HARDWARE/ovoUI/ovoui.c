#include "ovoui.h"
#include "font.h"


void Setting(void) {
    
}

OPTION MainMenu[] = {
    { {ICON48W+ICONSPACE, 0, ICON48W, ICON48H, OLED_MIX_XOR, (void*)0}, Setting },
    { {0, 0, ICON48W, ICON48H, OLED_MIX_XOR, (void *)0}, Setting },
    { {-(ICON48W+ICONSPACE), 0, ICON48W, ICON48H, OLED_MIX_XOR, (void*)0}, Setting },
};

void MenuInit(void){
    elementCount = sizeof(MainMenu) / sizeof(MainMenu[0]);
    for (u8 i = 0;i < elementCount;i++)elementPtrs[i] = &MainMenu[i].ele;
}
