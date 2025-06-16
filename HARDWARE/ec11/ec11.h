#ifndef __EC11_H
#define __EC11_H
#include "stm32f10x.h"

#define EC11_A_GPIO GPIOB
#define EC11_A_PIN GPIO_Pin_9
#define EC11_A_PIN_ID 9
#define EC11_A_PIN_RCC RCC_APB2Periph_GPIOB

#define EC11_B_GPIO GPIOB
#define EC11_B_PIN GPIO_Pin_8
#define EC11_B_PIN_ID 8
#define EC11_B_PIN_RCC RCC_APB2Periph_GPIOB

#define EC11_K_GPIO GPIOB
#define EC11_K_PIN GPIO_Pin_7
#define EC11_K_PIN_ID 7
#define EC11_K_PIN_RCC RCC_APB2Periph_GPIOB

#define EC11_A ((GPIOB->IDR & EC11_A_PIN) != (uint32_t)Bit_RESET)
#define EC11_B ((GPIOB->IDR & EC11_B_PIN) != (uint32_t)Bit_RESET)
#define EC11_K ((GPIOB->IDR & EC11_K_PIN) != (uint32_t)Bit_RESET)

#define EC11_LONG_PRESS 500 //长按判定时间(ms)
#define R_DIRECTION 0 //旋转方向调整

#define EC11TURNRIGTH 0x04
#define EC11TURNLEFT  0x02
#define EC11BUTTON 0x01
#define EC11BUTTONRIGTH 0x05
#define EC11BUTTONLEFT 0x03

/** EC11扫描返回值
 *  0000 0000
 *  低四位
 *  长按 8
 *  正转 4
 *  反转 2
 *  按下 1
 */

void EC11_Init(void);
u8 EC11_Scan(void);

extern u8 Ec11State;

#endif 
