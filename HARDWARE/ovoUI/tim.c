#include "tim.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"
#include "math.h"
#include "delay.h"

float xt = 0;
float yt = 0;
float Gxt = 0;

int16_t GlobalX = 0;
int16_t GlobalY = 0;
int16_t TargetX = 0;
int16_t TargetY = 0;

int16_t LeftEnd = 0;
int16_t RightEnd = 0;
int16_t animStartX; // 缓动计算起点
int16_t animStartY;

u8 cur_x1 = CURX;
u8 cur_y1 = CURY;
u8 cur_x2 = CURX + CURW;
u8 cur_y2 = CURY + CURH;

extern u8 keydown;

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
        u8 choice = 0;
        switch (Ec11Trigger) {
            case EC11TURNRIGTH:
                if(keydown)break;
                animStartX = GlobalX;
                TargetX += (ICONSPACE + ICON48W);
                xt = 0;





                break;
            case EC11TURNLEFT:
                // MenuOut();
                if(keydown)break;
                animStartX = GlobalX;
                TargetX -= (ICONSPACE + ICON48W);
                xt = 0;




                break;
            case EC11BUTTON:
                // TODO 转场动画完善
                if (GlobalX < TargetX - ICON48W/2 && GlobalX > TargetX + ICON48W/2)break;
                choice = (LeftEnd - GlobalX) / (ICONSPACE + ICON48W);
                if (menus[screen].opt[choice].action) {
                    animStartY = menus[screen].opt[0].ele.y;
                    TargetY = 64;
                    menus[screen].opt[choice].action();

                }
                
                
                
                
                break;
        }
        Ec11Trigger = 0;
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
    //if (menus[screen].opt[0].ele.y == 8) {    }

    if (GlobalX != TargetX) {
        cur_x1 = lerp(CURX - 4, CURX, Gxt);
        cur_x2 = lerp(CURX + CURW + 4, CURX + CURW, Gxt);
        cur_y1 = lerp(CURY + 4, CURY, Gxt);
        cur_y2 = lerp(CURY + CURH - 4, CURY + CURH, Gxt);
    }
    // *边缘限制
    if (GlobalX > LeftEnd + ICON48W / 2) {
        animStartX = GlobalX;
        TargetX = LeftEnd;
    }
    if (GlobalX < RightEnd - ICON48W / 2) {
        animStartX = GlobalX;
        TargetX = RightEnd;
    }
    // *X缓动
    if (GlobalX != TargetX) {
        xt += 0.003f;
        if (xt >= 1.0f) xt = 1.0f;
        Gxt = EASE_OUT(xt);
        GlobalX = lerp(animStartX, TargetX, Gxt);
        if (xt >= 1.0f) {
            GlobalX = TargetX;
            animStartX = TargetX;
            xt = 0;
            Gxt = 0;
        }
    } else {
        animStartX = TargetX = GlobalX;
    }
    // *Y缓动
    if (GlobalY != TargetY) {
        yt += 0.003f;
        if (yt >= 1.0f) yt = 1.0f;
        GlobalY = lerp(animStartY, TargetY,  EASE_OUT(yt));
        if (yt >= 1.0f) {
            GlobalY = TargetY;
            animStartY = TargetY;
            yt = 0;
        }
    }
}

void Position_Init(MENU menu) {
    cur_x1 = CURX;
    cur_y1 = CURY;
    cur_x2 = CURX + CURW;
    cur_y2 = CURY + CURH;

    GlobalX = menu.position;
    TargetX = menu.position;
    GlobalY = animStartY = 64;
    TargetY = 8;
    LeftEnd = menu.leftend;
    RightEnd = LeftEnd-(ICON48W+ICONSPACE)*(menu.optnum-1);
}