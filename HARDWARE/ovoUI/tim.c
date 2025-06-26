#include "tim.h"
#include "ec11.h"
#include "oled.h"
#include "ovoui.h"
#include "font.h"

void TIM3_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 4999;//arr 自动重装载
    TIM_TimeBaseStructure.TIM_Prescaler = 71;//psc 预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; // 子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM3, ENABLE);

    //PC13 LED
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    LED_OFF;
}

static inline void EaseOut(int16_t* value, int16_t target, u8 division) {
    if (*value == target) return;
    int16_t diff = target - *value;
    if (diff == 0) return;
    int16_t step = diff / division;
    if (step == 0) step = (diff > 0) ? 1 : -1;
    *value += step;
}

static inline void EaseOutU8(uint8_t* value, uint8_t target, uint8_t division) {
    if (*value == target) return;
    int16_t diff = (int16_t)target - (int16_t)(*value);
    int16_t step = diff / division;
    if (step == 0) step = (diff > 0) ? 1 : -1;
    int16_t result = (int16_t)(*value) + step;
    *value = (uint8_t)result;
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        static u8 prevState = 0;
        static u8 keyDown = 0;

        if (Ec11State & 0x04) {
            keyDown = 0;

            //右转
            On_Menu_Next();
        }
        else if (Ec11State & 0x02) {
            keyDown = 0;

            //左转
            On_Menu_Prev();
        }
        else {
            if (!(prevState & 0x01) && (Ec11State & 0x01)) {
                keyDown = 1;

                // 按键刚刚按下

            }
            else if ((prevState & 0x01) && !(Ec11State & 0x01)) {
                // 松开
                if (keyDown) {
                    keyDown = 0;

                    //按下后未旋转，抬起触发
                    if (menuState) {
                        if (menu.opt[MENUCHOICE].list[OPTIONCHOICE].action)
                            menu.opt[MENUCHOICE].list[OPTIONCHOICE].action();
                        return;
                    }
                    else On_Menu_Enter();

                }
            }
        }
        prevState = Ec11State;
        Ec11State = 0x00;


        // TODO 动画调整
        EaseOut(&menuOffsetX, menuOffsetX_Target, 5);

        // if主菜单边缘
        if (menuOffsetX > ICON48W / 2)menuOffsetX_Target = 0;
        if (menuOffsetX < -(menu.optnum - 1) * (ICON48W + ICONSPACE) - ICON48W / 2)
            menuOffsetX_Target = -(menu.optnum - 1) * (ICON48W + ICONSPACE);
        // if子菜单边缘
        if (optionOffset > 0)optionOffset = 0;
        if (optionOffset < -(menu.opt[MENUCHOICE].listnum - 4) * 16)
            optionOffset = -(menu.opt[MENUCHOICE].listnum - 4) * 16;


        for (u8 i = 0;i < menu.optnum;i++) {

            //文字动画 
            if (MENUCHOICE != i || menuState) {
                menu.opt[i].text.y = 64;
            }
            else {
                EaseOut(&menu.opt[i].text.y, 48, 16);
            }
            //主菜单绘制
            if (menuState) {
                if (i < MENUCHOICE)
                    EaseOut(&menu.opt[i].ele.x, -ICON48W, 12);
                if (i == MENUCHOICE)
                    EaseOut(&menu.opt[i].ele.x, -ICON48W / 2, 12);
                if (i > MENUCHOICE)
                    EaseOut(&menu.opt[i].ele.x, OLED_WIDTH, 12);
                EaseOut(&menu.opt[i].ele.y, 8, 20);
            }
            else {
                EaseOut(&menu.opt[i].ele.x, menuOffsetX + MENULEFTEND + (ICON48W + ICONSPACE) * i, 10);
                EaseOut(&menu.opt[i].ele.y, 0, 20); //主菜单动画偏移y
            }
        }

        for (u8 i = 0;i < menu.opt[MENUCHOICE].listnum;i++) {
            if (menuState)
                //子菜单绘制 
                EaseOut(&menu.opt[MENUCHOICE].list[i].text.y, i * 16 + optionOffset, 16);
            else
                //隐藏
                EaseOut(&menu.opt[MENUCHOICE].list[i].text.y, OLED_HEIGHT_PIXEL, 8);
        }

        if (menuState) {
            //光标部分
            EaseOutU8(&cursor.x0, menu.opt[MENUCHOICE].list[OPTIONCHOICE].text.x, 8);
            EaseOutU8(&cursor.y0, cursorOffset, 8);
            EaseOutU8(&cursor.x1, cursor.x0 + menu.opt[MENUCHOICE].list[OPTIONCHOICE].text.fontwidth, 8);
            EaseOutU8(&cursor.y1, cursor.y0 + 16, 8);
            //滚动条隐藏
            EaseOut(&scrollbarOffset, 0, 20);
            //滚动条滑块
            EaseOutU8(&scrollOffset, OLED_HEIGHT_PIXEL / (menu.opt[MENUCHOICE].listnum - 1) * OPTIONCHOICE, 20);
        }
        else {
            EaseOutU8(&cursor.x0, 40, 16);
            EaseOutU8(&cursor.y0, 0, 16);
            EaseOutU8(&cursor.x1, 40 + ICON48W, 16);
            EaseOutU8(&cursor.y1, ICON48H, 16);
            EaseOut(&scrollbarOffset, 8, 20);
            EaseOutU8(&scrollOffset, 0, 20);
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}