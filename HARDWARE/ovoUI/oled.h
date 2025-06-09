#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"

// **定义 OLED 屏幕尺寸**
#define WIDTH 128
#define HEIGHT 64

// **定义 ELEMENT 结构体**
typedef struct {
    u8 x, y;   // 元素位置（像素单位）
    u8 w, h;   // 元素宽度、高度（像素单位）
    u8 mix;    // 显示模式 0x00:覆盖 0x01:叠加 0x02:反色
    u8 *data;  // 显示数据指针
} ELEMENT;

// **定义链表结构**
typedef struct Node {
    ELEMENT ele;    // 存储 ELEMENT
    struct Node* next; // 指向下一个节点
} Node;

ELEMENT OLED_Create_Element(u8 x, u8 y, u8 w, u8 h, u8 mix, u8 *data);
void addElement(Node **head, ELEMENT newElement);
void modifyElement(ELEMENT *ele, u8 x, u8 y/*, u8 w, u8 h, u8 *data, u8 mix*/);
void modifyElementByData(Node *head, u8 *data, u8 x, u8 y);
void removeElement(Node **head, u8 x, u8 y);
void printElements(Node *head);
void OLED_Mix_Print(ELEMENT* ele, u8 num);
void OLED_Update();

#endif