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
    OPTION *opt;    //选项列表
    MENU_SHOW_MODE mode; //显示方式 //TODO 大图标显示和列表显示
    u8 optnum;  //选项个数
    u8 leftend; //左端
    int16_t position; //显示位置，用于记录上次选中
}MENU;

#define ICONSPACE 16

void _Main(void);
void Main_(void);
void _Model(void);
void Model_(void);

extern MENU menus[];
extern OPTION MainMenu[];
extern u8 screen;

#endif