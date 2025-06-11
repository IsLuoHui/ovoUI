#ifndef __TIM_H
#define __TIM_H
#include "stm32f10x.h"

#define LED_OFF (GPIOC->BSRR = GPIO_Pin_13) //H
#define LED_ON (GPIOC->BRR = GPIO_Pin_13)	//L

typedef struct {
    int Kp, Ki, Kd;  // PID 参数（整数）
    int prevError;   // 上一次误差
    int integral;    // 积分累计（整数）
} PID_Controller;
int PID_Compute(PID_Controller *pid, int target, int current);

void TIM3_Init(void);
void DrawShow(void);

extern int16_t GlobalX;

#endif
