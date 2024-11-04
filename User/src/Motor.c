#include "Motor.h"


/**
 * @fn      Motor_Init
 *
 * @brief   初始化电机
 *
 * @return none
 */
void Motor_Init(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure = {0};
    TIM_OCInitTypeDef       TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = 100;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 3599;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2,&TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM2,ENABLE);
    TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM2,ENABLE);
    TIM_Cmd(TIM2,ENABLE);
}


/**
 * @fn      Motor_Set_PWM
 *
 * @brief   设置电机的转速（占空比）
 *
 * @param   ccp 电机转速（占空比）
 * 
 * @return none
 */
void Motor_Set_PWM(u16 ccp)
{
    if(ccp >= 100)
        ccp = 100;
    TIM_SetCompare1(TIM2,ccp);
}


/**
 * @fn      Motor_Get_PWM
 *
 * @brief   发送1字节数据
 * 
 * @return  获取到的占空比
 */
u16 Motor_Get_PWM(void)
{
    return TIM_GetCapture1(TIM2);
}


/**
 * @fn      Motor_Set_Direction
 * 
 * @brief   设置前进后退
 *
 * @param   DIRECTION 方向
 * 
 * @return none
 */
void Moter_Set_Direction(DIRECTIONS DIRECTION)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, DIRECTION);
}
