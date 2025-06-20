#include "miic.h"

void OLED_IIC_GPIO_Init(void) {
    RCC->APB2ENR |= OLED_IIC_SCL_PIN_RCC | OLED_IIC_SDA_PIN_RCC;
    
// IIC SCL
#if OLED_IIC_SCL_PIN_ID < 8
    OLED_IIC_SCL_GPIO->CRL &= ~((u32)0xF << (4 * OLED_IIC_SCL_PIN_ID));  // 4位清0
    OLED_IIC_SCL_GPIO->CRL |=  ((u32)0x3 << (4 * OLED_IIC_SCL_PIN_ID));   // MODE6=11(50MHz)
    OLED_IIC_SCL_GPIO->CRL &= ~((u32)0xC << (4 * OLED_IIC_SCL_PIN_ID));  // CNF6=00(推挽输出)
#else 
    OLED_IIC_SCL_GPIO->CRH &= ~((u32)0xF << (4 * (OLED_IIC_SCL_PIN_ID - 8)));
    OLED_IIC_SCL_GPIO->CRH |=  ((u32)0x3 << (4 * (OLED_IIC_SCL_PIN_ID - 8)));
    OLED_IIC_SCL_GPIO->CRH &= ~((u32)0xC << (4 * (OLED_IIC_SCL_PIN_ID - 8)));
#endif

// IIC SDA
#if OLED_IIC_SDA_PIN_ID < 8
    OLED_IIC_SDA_GPIO->CRL &= ~((u32)0xF << (4 * OLED_IIC_SDA_PIN_ID));  // 4位清0
    OLED_IIC_SDA_GPIO->CRL |=  ((u32)0x3 << (4 * OLED_IIC_SDA_PIN_ID));   // MODE6=11(50MHz)
    OLED_IIC_SDA_GPIO->CRL &= ~((u32)0xC << (4 * OLED_IIC_SDA_PIN_ID));  // CNF6=00(推挽输出)
#else 
    OLED_IIC_SDA_GPIO->CRH &= ~((u32)0xF << (4 * (OLED_IIC_SDA_PIN_ID - 8)));
    OLED_IIC_SDA_GPIO->CRH |=  ((u32)0x3 << (4 * (OLED_IIC_SDA_PIN_ID - 8)));
    OLED_IIC_SDA_GPIO->CRH &= ~((u32)0xC << (4 * (OLED_IIC_SDA_PIN_ID - 8)));
#endif

    OLED_IIC_SCL_H;
    OLED_IIC_SDA_H;
}

void OLED_IIC_Init(void)
{
	OLED_IIC_W_CMD(0xAE);	//关闭显示
	OLED_IIC_W_CMD(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_IIC_W_CMD(0x80);	//刷新率 10~F0 越大越快
	OLED_IIC_W_CMD(0xA8);	//设置多路复用率
	OLED_IIC_W_CMD(0x3F);
	OLED_IIC_W_CMD(0xD3);	//设置显示偏移
	OLED_IIC_W_CMD(0x00);
	OLED_IIC_W_CMD(0x40);	//设置显示开始行
	OLED_IIC_W_CMD(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	OLED_IIC_W_CMD(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置
	OLED_IIC_W_CMD(0xDA);	//设置COM引脚硬件配置
	OLED_IIC_W_CMD(0x12);
	OLED_IIC_W_CMD(0x81);	//设置对比度控制
	OLED_IIC_W_CMD(0xCF);
	OLED_IIC_W_CMD(0xD9);	//设置预充电周期
	OLED_IIC_W_CMD(0xF1);
	OLED_IIC_W_CMD(0xDB);	//设置VCOMH取消选择级别
	OLED_IIC_W_CMD(0x30);
	OLED_IIC_W_CMD(0xA4);	//设置整个显示打开/关闭
	OLED_IIC_W_CMD(0xA6);	//设置A6正常/A7反色显示
	OLED_IIC_W_CMD(0x8D);	//设置充电泵
	OLED_IIC_W_CMD(0x14);
    OLED_IIC_W_CMD(0xAF);
}

void OLED_IIC_W_CMD(u8 cmd)
{
	OLED_IIC_Start();
	OLED_IIC_SendByte(OLED_ADDR);
	OLED_IIC_SendByte(0x00);
	OLED_IIC_SendByte(cmd); 
	OLED_IIC_Stop();
}

void OLED_IIC_W_DATA(u8 data)
{
	OLED_IIC_Start();
	OLED_IIC_SendByte(OLED_ADDR);
	OLED_IIC_SendByte(0x40);
	OLED_IIC_SendByte(data);
	OLED_IIC_Stop();
}


void OLED_IIC_Set_Cursor(u8 x, u8 page)
{
	OLED_IIC_W_CMD(0xB0 | page);
	OLED_IIC_W_CMD(0x10 | ((x & 0xF0) >> 4));
	OLED_IIC_W_CMD(x & 0x0F);
}
