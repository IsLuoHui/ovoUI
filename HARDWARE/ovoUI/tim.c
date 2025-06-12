#include "tim.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"
#include "math.h"

float t = 0;
float gt = 0;
int16_t GlobalX = 0;
int16_t TargetX = 0;
int16_t LeftEnd = 0;
int16_t RightEnd = 0;
int16_t animStartX; // 缓动计算起点

u8 cur_x1 = CURX;
u8 cur_y1 = CURY;
u8 cur_x2 = CURX + CURW;
u8 cur_y2 = CURY + CURH;

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
        switch (Ec11Trigger) {
            case EC11TURNRIGTH:
                animStartX = GlobalX;
                TargetX += (ICONSPACE + ICON48W);
                t = 0;
                Ec11Trigger = 0;
                break;
            case EC11TURNLEFT:
                animStartX = GlobalX;
                TargetX -= (ICONSPACE + ICON48W);
                t = 0;
                Ec11Trigger = 0;
                break;
            case EC11BUTTON:
                Ec11Trigger = 0;
                break;
            default:
                Ec11Trigger = 0;
                break;
        }

        if (Ec11Trigger) LED_ON;
        else LED_OFF;

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        DrawShow();
    }
}

int lerp(int start, int end, float t) {
    return start + (int)((end - start) * t);
}
float easeOut(float t) {
    return t * (2 - t);
}

void DrawShow(void) {
    //TODO 根据显示模式绘制
    //TODO 光标动效优化
    //if (GlobalX != TargetX) {
    //    cur_x1 = CURX - 4;
    //    cur_x2 = CURX + CURW + 4;
    //    cur_y1 = CURY + 4;
    //    cur_y2 = CURY + CURH - 4;
    //}
    //else {
    //    cur_x1 = CURX;
    //    cur_x2 = CURX + CURW;
    //    cur_y1 = CURY;
    //    cur_y2 = CURY + CURH;
    //}
    if (GlobalX != TargetX) {
        cur_x1 = lerp(CURX - 4, CURX, gt);
        cur_x2 = lerp(CURX + CURW + 4, CURX + CURW, gt);
        cur_y1 = lerp(CURY + 4, CURY, gt);
        cur_y2 = lerp(CURY + CURH - 4, CURY + CURH, gt);
    }
    if(GlobalX == TargetX&&cur_x1!=CURX) {
        cur_x1 = lerp(CURX, CURX - 4, gt);
        cur_x2 = lerp(CURX + CURW, CURX + CURW + 4, gt);
        cur_y1 = lerp(CURY, CURY + 4, gt);
        cur_y2 = lerp(CURY + CURH, CURY + CURH - 4, gt);
    }

    if (GlobalX > LeftEnd + ICON48W / 2) {
        animStartX = GlobalX;
        TargetX = LeftEnd;
    }
    if (GlobalX < RightEnd - ICON48W / 2) {
        animStartX = GlobalX;
        TargetX = RightEnd;
    }

    if (GlobalX != TargetX) {
        t += 0.003f;
        if (t >= 1.0f) t = 1.0f;
        gt = EASE_OUT(t);
        GlobalX = lerp(animStartX, TargetX, gt);
        if (t >= 1.0f) {
            GlobalX = TargetX;
            animStartX = TargetX;
            t = 0;
            gt = 0;
        }
    } else {
        animStartX = TargetX = GlobalX;
    }
}

void Position_Init(MENU menu) {
    GlobalX = menu.position;
    TargetX = menu.position;
    LeftEnd = menu.leftend;
    RightEnd = LeftEnd-(ICON48W+ICONSPACE)*(menu.optnum-1);
}