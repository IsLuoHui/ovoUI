#ifndef __OVOUI_H
#define __OVOUI_H
#include "stm32f10x.h"
#include "oled.h"

// *文字列表
typedef struct
{
    TEXT text;
    void (*action)(void);
}LIST;

// *图标列表
typedef struct
{
    TEXT text;
    ELEMENT ele; //选项元素
    LIST *list;
    u8 listnum; //列表个数
}OPTION;

// *图标菜单
typedef struct
{
    OPTION *opt;    //选项列表
    u8 optnum;  //选项个数
    u8 leftend; //左端
    int16_t position; //显示位置，用于记录上次选中
}MENU;

typedef struct {
    u8 x0,y0,x1,y1;
}CURSOR;

#define ICONSPACE 4

void Menu_Init(void);

extern MENU menu;
extern OPTION MainMenu[];
extern CURSOR cursor;

#endif