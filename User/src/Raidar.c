#include "Raidar.h"
#include "Delay.h"
#include "Command.h"


/**
 * @fn      Raidar_Init
 * 
 * @brief   初始化雷达
 * 
 * @return none
 */
void Raidar_Init(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure = {0};
    TIM_OCInitTypeDef       TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = 100;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 3599;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4,&TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM4,ENABLE);
    TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM4,ENABLE);
    TIM_Cmd(TIM4,ENABLE);

    UART_Init(115200,UART3);
    Delay_Init();
}


/**
 * @fn  Raidar_Set_Rotation
 * 
 * @brief 设置雷达转速
 * 
 * @param ccp 雷达转速（占空比）
 * 
 * @return none
 */
void Raidar_Set_Rotation(u16 ccp)
{
    if(ccp >= 100)
        ccp = 100;
    TIM_SetCompare1(TIM4,ccp);
}


/**
 * @fn      Raidar_Get_Rotation
 * 
 * @brief   获取雷达转速
 * 
 * 
 * @return  雷达转速
 */
u16 Raidar_Get_Rotation(void)
{
    return TIM_GetCapture1(TIM4);
}


/**
 * @fn  Raidar_Start_Scan
 * 
 * @brief   开始雷达扫描
 * 
 * @return none
 */
void Raidar_Start_Scan(Modes Mode)
{
    u8 Check = 0;
    if(Mode != Standard)
    {
        Echo[2] = 0x84;
    }
    Start_Command[3] = Mode;
    while(Check == 0)
    {
        Raidar_Stop_Scan();
        UART_Clear_Buffer(UART3);
        for(u8 i = 0;i <= 7;i++)
        {
            Check ^= Start_Command[i];
        }
        Start_Command[8] = Check;
        for(u8 i = 0;i <= 8;i++)
        {
            UART_Send_Data(UART3, Start_Command[i]);
        }
        Check = 1;
        for(u8 i = 0;i <= 6;i++)
        {
            if(UART_Get_Data(UART3) != Echo[i])
            {
                Check = 0;
                break;
            }
        }
    }
}


void Raidar_Stop_Scan(void)
{
    UART_Send_Data(UART3, 0xA5);
    UART_Send_Data(UART3, 0x25);
    Delay_Ms(1);
}
