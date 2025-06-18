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
}MENU;

typedef struct {
    u8 x0,y0,x1,y1;
}WINDOW;

extern int16_t menuOffsetX, menuOffsetX_Target;
extern int16_t optionOffset;
extern int16_t cursorOffset;
extern int16_t scrollbarOffset;
extern u8 scrollOffset;

#define ICONSPACE 4 //图标间距
#define MENULEFTEND 40 //菜单左端
#define MENUCHOICE (-menuOffsetX / (ICON48W + ICONSPACE)) //根据偏移计算当前选项
#define OPTIONCHOICE ((cursorOffset-optionOffset) / 16)

void Menu_Init(void);

extern MENU menu;
extern OPTION mainMenu[];
extern WINDOW cursor, window;



extern u8 menuState;

void On_Menu_Prev(void);
void On_Menu_Next(void);
void On_Menu_Enter(void);
void On_Menu_Back(void);

#endif