#ifndef __OLED_FONT_H
#define __OLED_FONT_H

/*Fixedsys ���� ���� ����ʽ*/
// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
extern const unsigned char ASCII_8X16[][16];

//���� ������������ģʽ
struct GB16
{
    unsigned char Data[32];
	unsigned char Index[2];
};
extern const struct GB16 GB2312_16X16[];


//ͼ�� ��ֱɨ�裬�����ң����ϵ��£���λ��ǰ
//48x48
extern const unsigned char ICON_48X48[][288];

#endif
