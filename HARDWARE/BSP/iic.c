#include "iic.h"
#include "font.h"
#define OLED_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)(x))
#define OLED_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)(x))
#define OLED_ADDR 0x78 //OLED模块IIC地址


void OLED_INIT(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_SCL(1);
	OLED_SDA(1);
}

void OLED_I2C_Start(void)
{
	OLED_SDA(1);
	OLED_SCL(1);
	OLED_SDA(0);
	OLED_SCL(0);
}

void OLED_I2C_Stop(void)
{
	OLED_SDA(0);
	OLED_SCL(1);
	OLED_SDA(1);
}

void OLED_I2C_SendByte(u8 byte)
{
	u8 i;
	for (i = 0; i < 8; i++)
	{
		if(byte & 0x80)OLED_SDA(1);
		else OLED_SDA(0);
		OLED_SCL(1);
		OLED_SCL(0);
		byte<<=1;
	}
	
	OLED_SCL(1);
	OLED_SCL(0);
}

void OLED_Command(u8 cmd)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(OLED_ADDR);
	OLED_I2C_SendByte(0x00);
	OLED_I2C_SendByte(cmd); 
	OLED_I2C_Stop();
}

void OLED_Data(u8 data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(OLED_ADDR);
	OLED_I2C_SendByte(0x40);
	OLED_I2C_SendByte(data);
	OLED_I2C_Stop();
}

void OLED_CMD_INIT(void)
{
	OLED_Command(0xAE);	//关闭显示
	OLED_Command(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_Command(0x80);	//刷新率 10~F0 越大越快
	OLED_Command(0xA8);	//设置多路复用率
	OLED_Command(0x3F);
	OLED_Command(0xD3);	//设置显示偏移
	OLED_Command(0x00);
	OLED_Command(0x40);	//设置显示开始行
	OLED_Command(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	OLED_Command(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置
	OLED_Command(0xDA);	//设置COM引脚硬件配置
	OLED_Command(0x12);
	OLED_Command(0x81);	//设置对比度控制
	OLED_Command(0xCF);
	OLED_Command(0xD9);	//设置预充电周期
	OLED_Command(0xF1);
	OLED_Command(0xDB);	//设置VCOMH取消选择级别
	OLED_Command(0x30);
	OLED_Command(0xA4);	//设置整个显示打开/关闭
	OLED_Command(0xA6);	//设置A6正常/A7反色显示
	OLED_Command(0x8D);	//设置充电泵
	OLED_Command(0x14);
	OLED_Command(0xAF);
}

void OLED_SetCursor(u8 x, u8 y)
{
	OLED_Command(0xB0 | y);
	OLED_Command(0x10 | ((x & 0xF0) >> 4));
	OLED_Command(x & 0x0F);
}

void OLED_CLS(void)
{  
	u8 i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(0, j);
		for(i = 0; i < 128; i++)
		{
			OLED_Data(0x00);
		}
	}
}

void OLED_ShowChar(u8 x, u8 y, char c)
{
	u8 i;
	OLED_SetCursor(x, y);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_Data(ASCII_8X16[c - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor(x,y+1);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_Data(ASCII_8X16[c - ' '][i + 8]);		//显示下半部分内容
	}
}

void OLED_ShowString(u8 x, u8 y, char *str)
{
	u8 i;
	for (i = 0; str[i] != '\0'; i++)
	{
		OLED_ShowChar(x+i*8, y, str[i]);
	}
}

/**
 *  @brief 计算`a`的`b`次幂
 *  @retval a^b
 */
u32 po(u32 a, u32 b)
{
	u32 r = 1;
	while (b--)
	{
		r *= a;
	}
	return r;
}

void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len)
{
	u8 i;
	for (i = 0; i < len; i++)							
	{
		OLED_ShowChar(x+i*8, y, num / po(10, len - i - 1) % 10 + '0');
	}
}

void OLED_ShowHexNum(u8 x, u8 y, u32 num, u8 len)
{
	u8 i, SingleNumber;
	for (i = 0; i < len; i++)							
	{
		SingleNumber = num / po(16, len - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(x + i * 8, y, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(x + i * 8, y, SingleNumber - 10 + 'A');
		}
	}
}