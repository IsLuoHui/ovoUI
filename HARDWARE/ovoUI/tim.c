#include "tim.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"

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

void On_EC11_Rotate_Right() {
    //右转
    On_Menu_Prev();
}
void On_EC11_Rotate_Left() {
    //左转
    On_Menu_Next();
}
void On_EC11_Press() {
    //按下触发
}
void On_EC11_Release() {
    //按下后未旋转，抬起触发
    On_Menu_Enter();
}

void TIM3_IRQHandler(void)
{ 	
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        static u8 prevState = 0;
        static u8 keyDown = 0;

        if (Ec11State & 0x04) {
            keyDown = 0;
            On_EC11_Rotate_Right();
        } else if (Ec11State & 0x02) {
            keyDown = 0;
            On_EC11_Rotate_Left();
        }
        else {
            if (!(prevState & 0x01) && (Ec11State & 0x01)) {
                // 按键刚刚按下
                keyDown = 1;
                On_EC11_Press();
            }
            else if ((prevState & 0x01) && !(Ec11State & 0x01)) {
                // 松开
                if (keyDown) {
                    keyDown = 0;
                    On_EC11_Release();  // 仅在未被打断时触发
                }
            }
        }
        prevState = Ec11State;
        Ec11State = 0x00;

        DrawShow();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}




void DrawShow(void) {
    // TODO 选项切换动画
    // TODO 转场动画
    // TODO 文字绘制
    // TODO 光标动效
    // TODO 边缘限制

    for (u8 i = 0;i < menu.optnum;i++){
        menu.opt[i].ele.x = menu.offset + MENULEFTEND + (ICON48W + ICONSPACE) * i;

        if (MENUCHOICE == i) {
            menu.opt[i].text.y = 48;
        }else{
            menu.opt[i].text.y = 64;
        }
    }
}

void Position_Init(MENU menu) {
    // TODO 位置信息初始化
    
}