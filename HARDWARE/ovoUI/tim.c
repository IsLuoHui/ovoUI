#include "tim.h"
#include "ec11.h"
#include "oled.h"
#include "font.h"
#include "stdlib.h"

//static inline int lerp(int start, int end, float t) {
//    return start + (int)((end - start) * t);
//}

void TIM3_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 4999;//arr 自动重装载
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
    if (menuState) {
        if(menu.opt[MENUCHOICE].list[OPTIONCHOICE].action)
            menu.opt[MENUCHOICE].list[OPTIONCHOICE].action();
        return;
    } else On_Menu_Enter();
}

void EaseOutSimple(int16_t *value, int16_t target,u8 division) {
    int16_t diff = target - *value;

    if (diff == 0) return;

    // 指数式减速
    int16_t step = diff/ division;

    // 至少移动一步，防止卡住
    if (step == 0) step = (diff > 0) ? 1 : -1;

    *value += step;
}

void EaseOutSimpleU8(uint8_t *value, uint8_t target, uint8_t division) {
    if (*value == target) return;

    int16_t diff = (int16_t)target - (int16_t)(*value);
    int16_t step = diff / division;

    if (step == 0) step = (diff > 0) ? 1 : -1;

    int16_t result = (int16_t)(*value) + step;

    // 裁剪结果，确保仍在 uint8_t 范围内
    if (result < 0) result = 0;
    if (result > 255) result = 255;

    *value = (uint8_t)result;
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

        
        // TODO 转场动画调整
        EaseOutSimple(&menuOffsetX, menuOffsetX_Target,5);

        //主菜单边缘
        if (menuOffsetX > ICON48W / 2)menuOffsetX_Target = 0;
        if (menuOffsetX < -(menu.optnum - 1) * (ICON48W + ICONSPACE) - ICON48W / 2)
            menuOffsetX_Target = -(menu.optnum - 1) * (ICON48W + ICONSPACE);
        //子菜单边缘
        if (optionOffset > 0)optionOffset = 0;
        if (optionOffset < -(menu.opt[MENUCHOICE].listnum-4) * 16)
            optionOffset = -(menu.opt[MENUCHOICE].listnum-4) * 16;


        for (u8 i = 0;i < menu.optnum;i++) {
            
            //文字动画 //TODO 文字动画效果
            if (MENUCHOICE != i || menuState) {
                menu.opt[i].text.y = 64;
            } else {
                EaseOutSimple(&menu.opt[i].text.y, 48, 10);
            }
            //主菜单绘制
            if (menuState) {
                if (i < MENUCHOICE)EaseOutSimple(&menu.opt[i].ele.x, -ICON48W, 8);
                    //menu.opt[i].ele.x = -ICON48W;
                if (i == MENUCHOICE)EaseOutSimple(&menu.opt[i].ele.x, -ICON48W/2,16);
                    //menu.opt[i].ele.x = -ICON48W / 2;
                if (i > MENUCHOICE)EaseOutSimple(&menu.opt[i].ele.x, OLED_WIDTH,16);
                    //menu.opt[i].ele.x = OLED_WIDTH;
            }else{
                EaseOutSimple(&menu.opt[i].ele.x, menuOffsetX + MENULEFTEND + (ICON48W + ICONSPACE) * i, 10);
                //menu.opt[i].ele.x = menuOffsetX + MENULEFTEND + (ICON48W + ICONSPACE) * i;
            }
            //子菜单动画偏移y
            EaseOutSimple(&menu.opt[i].ele.y, menuOffsetY, 24);
        }
        //子菜单绘制 // TODO 选项切换动画调整
        for (u8 i = 0;i < menu.opt[MENUCHOICE].listnum;i++) {
            if (menuState)EaseOutSimple(&menu.opt[MENUCHOICE].list[i].text.y, i * 16 + optionOffset, 16);
            //if (menuState)menu.opt[MENUCHOICE].list[i].text.y = i * 16+optionOffset;
            else EaseOutSimple(&menu.opt[MENUCHOICE].list[i].text.y, OLED_HEIGHT_PIXEL, 8);
            //else menu.opt[MENUCHOICE].list[i].text.y = OLED_HEIGHT_PIXEL;
        }
        //光标和滚动条部分 // TODO 光标动效调整
        if (menuState) {
            EaseOutSimpleU8(&cursor.x0, menu.opt[MENUCHOICE].list[OPTIONCHOICE].text.x, 8);
            EaseOutSimpleU8(&cursor.y0, cursorOffset, 8);
            EaseOutSimpleU8(&cursor.x1, cursor.x0 + menu.opt[MENUCHOICE].list[OPTIONCHOICE].text.fontwidth, 8);
            EaseOutSimpleU8(&cursor.y1, cursor.y0 + 16, 8);
            EaseOutSimple(&scrollbarOffset, 0, 20);
            EaseOutSimpleU8(&scrollOffset, OLED_HEIGHT_PIXEL / (menu.opt[MENUCHOICE].listnum - 1) * OPTIONCHOICE, 20);
            //cursor.x0 = menu.opt[MENUCHOICE].list[OPTIONCHOICE].text.x;
            //cursor.y0 = cursorOffset;
            //cursor.x1 = cursor.x0 + menu.opt[MENUCHOICE].list[OPTIONCHOICE].text.fontwidth;
            //cursor.y1 = cursor.y0 + 16;
            //scrollbarOffset = 0;
        }
        else {
            EaseOutSimpleU8(&cursor.x0, 40, 16);
            EaseOutSimpleU8(&cursor.y0, 0, 16);
            EaseOutSimpleU8(&cursor.x1, 40 + ICON48W, 16);
            EaseOutSimpleU8(&cursor.y1, ICON48H, 16);
            EaseOutSimple(&scrollbarOffset, 8, 20);
            EaseOutSimpleU8(&scrollOffset, 0, 20);
            
            //cursor.x0 = 40;
            //cursor.y0 = 0;
            //cursor.x1 = 40 + ICON48W;
            //cursor.y1 = ICON48H;
            //scrollbarOffset = 8;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}