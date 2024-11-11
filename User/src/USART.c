#include "USART.h"

UART_Buffer UART_Buffer1,UART_Buffer2,UART_Buffer3 = {0};

/**
 * @fn      UART_Init
 *
 * @brief   初始化指定的USART模块
 *
 * @param   baudrate 波特率
 * @param   UART_Select 指定UASRT
 * 
 * @return none
 */
void UART_Init(u32 baudrate,UARTS UART_Select)
{
    DMA_InitTypeDef  DMA_InitStructure = {0};
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //共用DMA结构体值定义
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize         = RX_BUFFER_LEN;

    //GPIO替代使能
    switch(UART_Select)
    {
        case UART1:
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            GPIO_Init(GPIOA, &GPIO_InitStructure);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            GPIO_Init(GPIOA, &GPIO_InitStructure);
            break;
        }
        case UART2:
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            GPIO_Init(GPIOA, &GPIO_InitStructure);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            GPIO_Init(GPIOA, &GPIO_InitStructure);
            break;
        }
        case UART3:
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            GPIO_Init(GPIOB, &GPIO_InitStructure);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            GPIO_Init(GPIOB, &GPIO_InitStructure);
            break;
        }
        default:break;
    }

    //中断仲裁器共用结构体值定义
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

    //USART共用结构体值定义
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    //剩余不同USART定义
    switch(UART_Select)
    {
        case UART1:
        {
            DMA_DeInit(USART1_RX_CH);
            USART_Init(USART1, &USART_InitStructure);
            USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

            NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;

            NVIC_Init(&NVIC_InitStructure);

            USART_Cmd(USART1, ENABLE);

            DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DATAR);
            DMA_InitStructure.DMA_MemoryBaseAddr     = (u32)UART_Buffer1.RX_Buffer;

            DMA_Init(USART1_RX_CH, &DMA_InitStructure);

            DMA_ITConfig(USART1_RX_CH, DMA_IT_TC | DMA_IT_HT, ENABLE);
            NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel5_IRQn;
            NVIC_Init(&NVIC_InitStructure);

            DMA_Cmd(USART1_RX_CH, ENABLE);
            USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
            break;
        }
        case UART2:
        {
            DMA_DeInit(USART2_RX_CH);
            USART_Init(USART2, &USART_InitStructure);
            USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);

            NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;

            NVIC_Init(&NVIC_InitStructure);

            USART_Cmd(USART2, ENABLE);

            DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DATAR);
            DMA_InitStructure.DMA_MemoryBaseAddr     = (u32)UART_Buffer2.RX_Buffer;

            DMA_Init(USART2_RX_CH, &DMA_InitStructure);

            DMA_ITConfig(USART2_RX_CH, DMA_IT_TC | DMA_IT_HT, ENABLE);
            NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel6_IRQn;
            NVIC_Init(&NVIC_InitStructure);

            DMA_Cmd(USART2_RX_CH, ENABLE);
            USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
            break;
        }
        case UART3:
        {
            DMA_DeInit(USART3_RX_CH);
            USART_Init(USART3, &USART_InitStructure);
            USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);

            NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
            NVIC_Init(&NVIC_InitStructure);
            USART_Cmd(USART3, ENABLE);

            DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART3->DATAR);
            DMA_InitStructure.DMA_MemoryBaseAddr     = (u32)UART_Buffer3.RX_Buffer;

            DMA_Init(USART3_RX_CH, &DMA_InitStructure);

            DMA_ITConfig(USART3_RX_CH, DMA_IT_TC | DMA_IT_HT, ENABLE);
            NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel3_IRQn;
            NVIC_Init(&NVIC_InitStructure);
            
            DMA_Cmd(USART3_RX_CH, ENABLE);
            USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
            break;
        }
        default:break;
    }
}

/**
 * @fn      UART_Clear_Buffer
 *
 * @brief   
 *
 * @param   baudrate 波特率
 * @param   UART_Select 指定UASRT
 * 
 * @return none
 */
void UART_Clear_Buffer(UARTS UART_Select)
{
    switch(UART_Select)
    {
        case UART1:UART_Buffer1.Start_Conter = UART_Buffer1.End_Conter;break;
        case UART2:UART_Buffer2.Start_Conter = UART_Buffer2.End_Conter;break;
        case UART3:UART_Buffer3.Start_Conter = UART_Buffer3.End_Conter;break;
    }
}

/**
 * @fn      UART_Send_Data
 *
 * @brief   发送1字节数据
 *
 * @param   UART_Select 指定UASRT
 * @param   Data 发送的代码
 * 
 */
void UART_Send_Data(UARTS UART_Select,u8 Data)
{
    switch(UART_Select)
    {
        case UART1:
        {
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
            USART_SendData(USART1,Data);
            break;
        }
        case UART2:
        {
            while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
            USART_SendData(USART2,Data);
            break;
        }
        case UART3:
        {
            while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
            USART_SendData(USART3,Data);
            break;
        }
    }
}

/**
 * @fn      UART_Get_Data
 *
 * @brief   从缓冲区获取1字节的数据
 *
 * @param   UART_Select 指定UASRT
 * 
 * @return  获取的数据
 */
u8 UART_Get_Data(UARTS UART_Select)
{
    u8 data = 0;
    while(UART_Get_Length(UART_Select) == 0);
    switch(UART_Select)
    {
        case UART1:
        {
            data = UART_Buffer1.RX_Buffer[UART_Buffer1.Start_Conter];
            UART_Buffer1.Start_Conter++;
            if(UART_Buffer1.Start_Conter == RX_BUFFER_LEN)
            UART_Buffer1.Start_Conter = 0;
            break;
        }
        case UART2:
        {
            data = UART_Buffer2.RX_Buffer[UART_Buffer2.Start_Conter];
            UART_Buffer2.Start_Conter++;
            if(UART_Buffer2.Start_Conter == RX_BUFFER_LEN)
            UART_Buffer2.Start_Conter = 0;
            break;
        }
        case UART3:
        {
            data = UART_Buffer3.RX_Buffer[UART_Buffer3.Start_Conter];
            UART_Buffer3.Start_Conter++;
            if(UART_Buffer3.Start_Conter == RX_BUFFER_LEN)
            UART_Buffer3.Start_Conter = 0;
            break;
        }
        default:break;
    }
    return data;
}

/**
 * @fn      UART_Get_Length
 *
 * @brief   获取缓冲区未处理的字节数
 *
 * @param   UART_Select 指定UASRT
 * 
 * @return  字节数量
 */
u16 UART_Get_Length(UARTS UART_Select)
{
    switch(UART_Select)
    {
        case UART1:
        {
            return (UART_Buffer1.End_Conter < UART_Buffer1.Start_Conter) ? (RX_BUFFER_LEN + UART_Buffer1.End_Conter - UART_Buffer1.Start_Conter) : (UART_Buffer1.End_Conter - UART_Buffer1.Start_Conter);
        }
        case UART2:
        {
            return (UART_Buffer2.End_Conter < UART_Buffer2.Start_Conter) ? (RX_BUFFER_LEN + UART_Buffer2.End_Conter - UART_Buffer2.Start_Conter) : (UART_Buffer2.End_Conter - UART_Buffer2.Start_Conter);
        }
        case UART3:
        {
            return (UART_Buffer3.End_Conter < UART_Buffer3.Start_Conter) ? (RX_BUFFER_LEN + UART_Buffer3.End_Conter - UART_Buffer3.Start_Conter) : (UART_Buffer3.End_Conter - UART_Buffer3.Start_Conter);
        }
        default:return 0;
    }
}

/**
 * @fn      UART_Send_Array
 *
 * @brief   发送一个数组
 *
 * @param   UART_Select 指定UASRT
 * @param   Array 数组指针（uint_8类型）
 * @param   Length 发送个数
 * 
 * @return none
 */
void UART_Send_Array(UARTS UART_Select,u8 *Array,int Length)
{
    for(u16 i =0;i <= Length - 1;i++)
    {
        UART_Send_Data(UART_Select,*(Array + i));
    }
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        UART_Buffer1.End_Conter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART1_RX_CH);
        if(UART_Buffer1.End_Conter == RX_BUFFER_LEN)
        UART_Buffer1.End_Conter = 0;
        USART_ReceiveData(USART1);
    }
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        UART_Buffer2.End_Conter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART2_RX_CH);
        if(UART_Buffer2.End_Conter == RX_BUFFER_LEN)
        UART_Buffer2.End_Conter = 0;
        USART_ReceiveData(USART2);
    }
}

void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
    {
        UART_Buffer3.End_Conter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART3_RX_CH);
        if(UART_Buffer3.End_Conter == RX_BUFFER_LEN)
        UART_Buffer3.End_Conter = 0;
        USART_ReceiveData(USART3);
    }
}

void DMA1_Channel5_IRQHandler(void)
{
    UART_Buffer1.End_Conter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART1_RX_CH);
    if(UART_Buffer1.End_Conter == RX_BUFFER_LEN)
    UART_Buffer1.End_Conter = 0;
    DMA_ClearITPendingBit(DMA1_IT_TC5 | DMA1_IT_HT5);
}

void DMA1_Channel6_IRQHandler(void)
{
    UART_Buffer2.End_Conter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART2_RX_CH);
    if(UART_Buffer2.End_Conter == RX_BUFFER_LEN)
    UART_Buffer2.End_Conter = 0;
    DMA_ClearITPendingBit(DMA1_IT_TC6 | DMA1_IT_HT6);
}

void DMA1_Channel3_IRQHandler(void)
{
    UART_Buffer3.End_Conter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART3_RX_CH);
    if(UART_Buffer3.End_Conter == RX_BUFFER_LEN)
    UART_Buffer3.End_Conter = 0;
    DMA_ClearITPendingBit(DMA1_IT_TC3 | DMA1_IT_HT3);
}
