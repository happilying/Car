#include "Light.h"

void Light_Init(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_WriteBit(GPIOB,GPIO_Pin_12,0);
}

void Light_Set_Status(Status State)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_12,State);
}