#ifndef __OVOUI_H
#define __OVOUI_H
#include "stm32f10x.h"
#include "oled.h"

typedef struct
{
    TEXT text;
    void (*action)(void);
}LIST;

typedef struct
{
    TEXT text;
    ELEMENT ele; //选项元素
    LIST *list;
    u8 listnum; //列表个数
}OPTION;

typedef struct
{
    OPTION *opt;    //选项列表
    u8 optnum;  //选项个数
    u8 leftend; //左端
    int16_t position; //显示位置，用于记录上次选中
}MENU;

typedef struct {
    u8 x1,y1,x2,y2;
}CURSOR;

#define ICONSPACE 16

void Menu_Init(void);

extern MENU menu;
extern OPTION MainMenu[];

#endif