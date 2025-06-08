#ifndef __EC11_H
#define __EC11_H
#include "stm32f10x.h"

/** EC11扫描返回值
 *  0000 0000
 *  低四位
 *  长按 8
 *  正转 4
 *  反转 2
 *  按下 1
 */

#define EC11_A ((GPIOB->IDR & GPIO_Pin_9) != (uint32_t)Bit_RESET)
#define EC11_B ((GPIOB->IDR & GPIO_Pin_8) != (uint32_t)Bit_RESET)
#define EC11_K ((GPIOB->IDR & GPIO_Pin_7) != (uint32_t)Bit_RESET)

#define EC11_LONG_PRESS 500 //长按判定时间(ms)
#define R_DIRECTION 1 //旋转方向

void EC11_Init(void);
u8 EC11_Scan(void);

#endif
