#ifndef __OLED_FONT_H
#define __OLED_FONT_H

/*Fixedsys 阴码 逆向 列行式*/
// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
extern const unsigned char ASCII_8X16[][16];

//汉字 表格后索引紧凑模式
struct GB16
{
    unsigned char Data[32];
	unsigned char Index[2];
};
extern const struct GB16 GB2312_16X16[];


//图标 垂直扫描，从左到右，从上到下，高位在前
//48x48
extern const unsigned char ICON_48X48[][288];

#endif
