#ifndef __OVOUI_H
#define __OVOUI_H
#include "stm32f10x.h"
#include "oled.h"

typedef struct
{
    ELEMENT ele;
    void (*action)(void);
}OPTION;

typedef enum {
    MENU_SHOW_Horizontal = 0x01,
    MENU_SHOW_Vertical = 0x02,
}MENU_SHOW_MODE;

typedef struct
{
    OPTION *opt;
    MENU_SHOW_MODE mode;
    u8 optnum;
    int16_t position;
}MENU;


#define ICONSPACE 24

void MainMenuInit(void);

extern MENU menus[1];
extern OPTION MainMenu[];

#endif