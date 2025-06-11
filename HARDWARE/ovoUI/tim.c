#include "tim.h"
#include "ec11.h"
#include "oled.h"
#include "ovoui.h"
#include "font.h"
#include "math.h"

u8 left = 0;
u8 right = 0;
float t = 0;

int16_t GlobalX = 40;
int16_t TargetX;
int16_t temp_GX;

void TIM3_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 999;//arr 自动重装载
    TIM_TimeBaseStructure.TIM_Prescaler = 71;//psc 预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; // 子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM3, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    LED_OFF;
}

void TIM3_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//是更新中断
    {
        if (right == 1 || left == 1) {
            TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
            DrawShow();
            return;
        }
        
        switch (Ec11Trigger) {
            case EC11TURNRIGTH:
                if (right || left)break;
                right = 1;
                temp_GX = GlobalX;
                TargetX = GlobalX + ICONSPACE + ICON48W;
                t = 0;

                break;
            case EC11TURNLEFT:
                if (right || left)break;
                temp_GX = GlobalX;
                TargetX = GlobalX - (ICONSPACE + ICON48W);
                left = 1;
                t = 0;
                
                break;
            case EC11BUTTON:
                if (right || left)break;

                Ec11Trigger = 0x00;
                break;
            default:
                Ec11Trigger = 0x00;
                break;
        }

        if (Ec11Trigger)LED_ON;
        else LED_OFF;









        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIM4更新中断标志
	}
}

int lerp(int start, int end, float t) {
    return start + (int)((end - start) * t);
}
float easeOut(float t) {
    return 1 - (1 - t) * (1 - t); // 先快后慢
}
float easeIn(float t) {
    return t * t;  // 先慢后快（二次函数）
}
float easeInExpo(float t) {
    return (t == 0) ? 0 : pow(2, 10 * (t - 1));  // 指数增长
}
float easeInOut(float t) {
    return (t < 0.5) ? 2 * t * t : 1 - pow(-2 * t + 2, 2) / 2;
}


void DrawShow(void) {
    // TODO 动画效果
    if (right) {
        t += 0.004f;
        if (t>=1.0f) {
            right = 0;
            Ec11Trigger = 0;
            t = 0;
            GlobalX=TargetX;
            return;
        }
        GlobalX=lerp(temp_GX, TargetX, easeOut(t));
        //GlobalX += 1;
        //temp += 1;
    }
    if (left) {
        t += 0.005f;
        if (t>=1.0f) {
            left = 0;
            Ec11Trigger = 0;
            t = 0;
            GlobalX=TargetX;
            return;
        }
        GlobalX=lerp(temp_GX, TargetX, easeInExpo(t));
    }
}

