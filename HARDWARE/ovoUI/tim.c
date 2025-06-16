#include "tim.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"


u8 tT = 0;
int16_t GlobalX = 0;

//static inline int lerp(int start, int end, float t) {
//    return start + (int)((end - start) * t);
//}

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
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        switch (Ec11Trigger){
            // TODO 选项切换动画 
            case EC11TURNRIGTH:
                GlobalX += (ICONSPACE + ICON48W);
                break;
            case EC11TURNLEFT:
                GlobalX -= (ICONSPACE + ICON48W);
                break;
            case EC11BUTTON:
                // TODO 转场动画

                break;
        }
        tT = Ec11Trigger;
        Ec11Trigger = 0;
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        DrawShow();
    }
}




void DrawShow(void) {
    //TODO 文字绘制
    //TODO 光标动效
    //TODO 边缘限制

}

void Position_Init(MENU menu) {
    // TODO 位置信息初始化
    GlobalX = menu.position;
    
}