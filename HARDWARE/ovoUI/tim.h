#ifndef __TIM_H
#define __TIM_H
#include "stm32f10x.h"

#define LED_OFF (GPIOC->BSRR = GPIO_Pin_13) //H
#define LED_ON (GPIOC->BRR = GPIO_Pin_13)	//L

void TIM3_Init(void);

#endif
