#ifndef __OVOUI_H
#define __OVOUI_H
#include "stm32f10x.h"
#include "oled.h"

typedef struct
{
    ELEMENT ele;
    void (*action)(void);
}OPTION;

#define ICONSPACE 24

void MenuInit(void);

extern OPTION MainMenu[];


#endif