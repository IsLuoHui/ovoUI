#include "stm32f10x.h"
#ifndef __IIC_H
#define __IIC_H

/**
 *  @brief 初始化OLED IIC引脚,`PA11`模拟`SCL`,`PA12`模拟`SDA`
 */
void OLED_INIT(void);

/**
 *  @brief IIC发送开始信号
 */
void OLED_I2C_Start(void);

/**
 *  @brief IIC发送结束信号
 */
void OLED_I2C_Stop(void);

/**
 *  @brief IIC发送一个字节
 *  @param byte 一字节数据
 */
void OLED_I2C_SendByte(u8 byte);

/**
 *  @brief IIC发送OLED控制命令
 *  @param cmd OLED控制码
 */
void OLED_Command(u8 cmd);

/**
 *  @brief IIC发送OLED显示数据
 *  @param data OLED显示数据
 */
void OLED_Data(u8 data);

/**
 *  @brief IIC发送OLED初始化命令
 */
void OLED_CMD_INIT(void);

/**
 *  @brief 设置OLED屏幕指针位置
 *  @param x 列,范围[0,127]
 *  @param y 页,范围[0,7]
 */
void OLED_SetCursor(u8 x, u8 y);

/**
 *  @brief OLED清屏
 */
void OLED_CLS(void);

/**
 *  @brief OLED显示一个字符
 *  @param x 列,范围[0,127]
 *  @param y 页,范围[0,7]
 *  @param c 字符
 */
void OLED_ShowChar(u8 x, u8 y, char c);

/**
 *  @brief OLED显示字符串
 *  @param x 列,范围[0,127]
 *  @param y 页,范围[0,7]
 *  @param str 字符串
 */
void OLED_ShowString(u8 x, u8 y, char *str);

/**
 *  @brief OLED显示数字（十进制，正数）
 *  @param x 列,范围[0,127]
 *  @param y 页,范围[0,7]
 *  @param num 要显示的数字,范围[0,4294967295]
 *  @param len 数字长度,不足左边补0,范围[1,10]
 */
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len);

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  x 列,范围[0,127]
  * @param  y 页,范围[0,7]
  * @param  num 要显示的数字,范围[0,0xFFFFFFFF]
  * @param  len 数字长度,范围[1,8]
  */
void OLED_ShowHexNum(u8 x, u8 y, u32 num, u8 len);

#endif
