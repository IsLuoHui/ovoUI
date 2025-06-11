#include "tim.h"
#include "ec11.h"
#include "oled.h"
#include "ovoui.h"
#include "font.h"

u8 left = 0;
u8 right = 0;
u8 temp = 0;

extern int16_t GlobalX = 40;


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
                break;
            case EC11TURNLEFT:
                if (right || left)break;
                left = 1;
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

void Animation(u8 status) {

}

void DrawShow(void) {
    // TODO 动画效果
    if (right) {
        if (temp == (ICONSPACE + ICON48W)) {
            temp = 0;
            right = 0;
            Ec11Trigger = 0;
            return;
        }
        GlobalX += 1;
        temp += 1;
    }
    if (left) {
        if (temp == (ICONSPACE + ICON48W)) {
            temp = 0;
            left = 0;
            Ec11Trigger = 0;
            return;
        }
        GlobalX -= 1;
        temp += 1;
    }
}