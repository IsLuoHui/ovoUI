#include "ec11.h"
#include "spi.h"

extern u8 ec11;
extern int16_t x, y;

static u8 EC11_AL;
u8 OLEDSHOW = 1;

void EC11_Init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 999;//arr 自动重装载
    TIM_TimeBaseStructure.TIM_Prescaler = 71;//psc 预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );

    EC11_AL = EC11_A;   //避免初始化后触发一次

    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;; // 子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM4, ENABLE);
}


u8 EC11_Scan(void) {
    
    static u8 ScanResult;
    static u16 DLTime;

    ScanResult = 0;
    if (!EC11_K) {
        ScanResult |= 0x01;  //按下
        if (DLTime == 0)ScanResult |= 0x08; //长按
        if (DLTime > 0)DLTime--;
    }
    else DLTime = EC11_LONG_PRESS;
    //没有转动
    if (EC11_A == EC11_AL)return ScanResult;

#if R_DIRECTION
    if (EC11_A) {
        if (EC11_B == 1) ScanResult |= 0x04; //正转
        else ScanResult |= 0x02; //反转
    }
#else
    if (!EC11_A) {
        if (EC11_B == 1) ScanResult |= 0x04; //正转
        else ScanResult |= 0x02; //反转
    }
#endif

    EC11_AL = EC11_A;   //更新暂存
    return ScanResult;
}

void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
    {
        ec11 = EC11_Scan();
        if (ec11 & 0x04 && !(ec11 & 0x01)) {
            x++;
            OLEDSHOW = 1;
        }
        if (ec11 & 0x02 && !(ec11 & 0x01)) {
            x--;
            OLEDSHOW = 1;
        }
        if (ec11 & 0x04 && ec11 & 0x01) {
            y++;
            OLEDSHOW = 1;
        }
        if (ec11 & 0x02 && ec11 & 0x01) {
            y--;
            OLEDSHOW = 1;
        }
        //if (!(ec11 & 0x06) && ec11 & 0x01)OLEDSHOW = 0;
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIM4更新中断标志    
	}
}