#include "ec11.h"
#include "spi.h"

extern u8 ec11;
extern int16_t x, y;

static u8 EC11_AL;
u8 OLEDSHOW = 1;

void EC11_Init(void) {
    RCC->APB2ENR |= EC11_A_PIN_RCC | EC11_B_PIN_RCC | EC11_K_PIN_RCC;
//EC11_A
#if EC11_A_PIN_ID < 8
    EC11_A_GPIO->CRL &= ~((u32)0xF << (4 * EC11_A_PIN_ID));  // 清零4位
    EC11_A_GPIO->CRL |=  ((u32)0x8 << (4 * EC11_A_PIN_ID));  // CNF=10, MODE=00
#else 
    EC11_A_GPIO->CRH &= ~((u32)0xF << (4 * (EC11_A_PIN_ID - 8)));
    EC11_A_GPIO->CRH |=  ((u32)0x8 << (4 * (EC11_A_PIN_ID - 8)));
#endif
//EC11_B
#if EC11_B_PIN_ID < 8
    EC11_B_GPIO->CRL &= ~((u32)0xF << (4 * EC11_B_PIN_ID));  // 清零4位
    EC11_B_GPIO->CRL |=  ((u32)0x8 << (4 * EC11_B_PIN_ID));  // CNF=10, MODE=00
#else 
    EC11_B_GPIO->CRH &= ~((u32)0xF << (4 * (EC11_B_PIN_ID - 8)));
    EC11_B_GPIO->CRH |=  ((u32)0x8 << (4 * (EC11_B_PIN_ID - 8)));
#endif
//EC11_K
#if EC11_K_PIN_ID < 8
    EC11_K_GPIO->CRL &= ~((u32)0xF << (4 * EC11_K_PIN_ID));  // 清零4位
    EC11_K_GPIO->CRL |=  ((u32)0x8 << (4 * EC11_K_PIN_ID));  // CNF=10, MODE=00
#else 
    EC11_K_GPIO->CRH &= ~((u32)0xF << (4 * (EC11_K_PIN_ID - 8)));
    EC11_K_GPIO->CRH |=  ((u32)0x8 << (4 * (EC11_K_PIN_ID - 8)));
#endif

    EC11_A_GPIO->BSRR = (1 << EC11_A_PIN_ID);
    EC11_B_GPIO->BSRR = (1 << EC11_B_PIN_ID);
    EC11_K_GPIO->BSRR = (1 << EC11_K_PIN_ID);

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