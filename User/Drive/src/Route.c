#include "Route.h"

/**
 * @fn      Route_Init
 *
 * @brief   初始化舵机模块
 * 
 * @return none
 */
void Route_Init(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure = {0};
    TIM_OCInitTypeDef       TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = 2000;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 719;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CENTER;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM3, ENABLE);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @fn      Route_Set_Angle
 *
 * @brief   设置舵机转动角度
 *
 * @param   Angle 角度(-90~90度)
 * 
 * @return none
 */
void Route_Set_Angle(float Angles)
{
    u8 Angle = Angles / 45 * 50 + CENTER;
    if(Angle > MAX_ROTATION)
    Angle = MAX_ROTATION;
    else if(Angle < MIN_ROTATION)
    Angle = MIN_ROTATION;
    TIM_SetCompare1(TIM3,Angle);
}

/**
 * @fn      Route_Get_Angle
 *
 * @brief   获取舵机的当前角度
 * 
 * @return  角度(-90~90度)
 */
float Route_Get_Angle(void)
{
    return ((int)TIM_GetCapture1(TIM4) - CENTER)/50.0f * 45;
}
