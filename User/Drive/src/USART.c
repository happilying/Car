#include "USART.h"
#include "Location.h"
//#include "GPS.h"

UART_Buffer UART_Buffer1 = {0},UART_Buffer2 = {0},UART_Buffer3 = {0};

extern void HardFault_Handler(void);

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
void UART_Init(UARTS UART_Select,u32 baudrate)
{
    //初始化结构体定义
    DMA_InitTypeDef  DMA_InitStructure = {0};
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    //DMA时钟使能
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
 * @brief   清除指定UART的缓冲区
 *
 * @param   UART_Select 指定UASRT
 * 
 * @return  none
 */
void UART_Clear_Buffer(UARTS UART_Select)
{
    switch(UART_Select)
    {
        case UART1:UART_Buffer1.Start_Counter = UART_Buffer1.End_Counter;break;
        case UART2:UART_Buffer2.Start_Counter = UART_Buffer2.End_Counter;break;
        case UART3:UART_Buffer3.Start_Counter = UART_Buffer3.End_Counter;break;
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
 * @param   UART_Select 指定USART
 *
 * @return  获取的数据
 */
u8 UART_Get_Data(UARTS UART_Select)
{
    u8 data = 0;
    if(UART_Get_Length(UART_Select) == 0)
    {
        return data;
    }
    switch (UART_Select)
    {
        case UART1:
        {
            data = UART_Buffer1.RX_Buffer[UART_Buffer1.Start_Counter];
            UART_Buffer1.Start_Counter++;
            if(UART_Buffer1.Start_Counter == RX_BUFFER_LEN)
            UART_Buffer1.Start_Counter = 0;
            break;
        }
        case UART2:
        {
            data = UART_Buffer2.RX_Buffer[UART_Buffer2.Start_Counter];
            UART_Buffer2.Start_Counter++;
            if(UART_Buffer2.Start_Counter == RX_BUFFER_LEN)
            UART_Buffer2.Start_Counter = 0;
            break;
        }
        case UART3:
        {
            data = UART_Buffer3.RX_Buffer[UART_Buffer3.Start_Counter];
            UART_Buffer3.Start_Counter++;
            if(UART_Buffer3.Start_Counter == RX_BUFFER_LEN)
            UART_Buffer3.Start_Counter = 0;
            break;
        }
        default:break;
    }
    return data;
}

/**
 * @fn      UART_Get_Data_With_Position
 *
 * @brief   从缓冲区获取1字节的数据
 *
 * @param   UART_Select 指定USART
 * @param   position    位置
 *
 * @return  获取的数据
 */
u8 UART_Get_Data_With_Position(UARTS UART_Select, int position)
{
    if(UART_Get_Length(UART_Select) < position)
        return 0;
    for(u8 i = 0;i < position;i++)
    {
        UART_Get_Data(UART_Select);
    }
    return UART_Get_Data(UART_Select);
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
            return (UART_Buffer1.End_Counter < UART_Buffer1.Start_Counter) ? (RX_BUFFER_LEN + UART_Buffer1.End_Counter - UART_Buffer1.Start_Counter) : (UART_Buffer1.End_Counter - UART_Buffer1.Start_Counter);
        }
        case UART2:
        {
            return (UART_Buffer2.End_Counter < UART_Buffer2.Start_Counter) ? (RX_BUFFER_LEN + UART_Buffer2.End_Counter - UART_Buffer2.Start_Counter) : (UART_Buffer2.End_Counter - UART_Buffer2.Start_Counter);
        }
        case UART3:
        {
            return (UART_Buffer3.End_Counter < UART_Buffer3.Start_Counter) ? (RX_BUFFER_LEN + UART_Buffer3.End_Counter - UART_Buffer3.Start_Counter) : (UART_Buffer3.End_Counter - UART_Buffer3.Start_Counter);
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

/**
 * @fn      UART_Set_baudrate
 *
 * @brief   更改UART波特率
 *
 * @param   UART_Select 指定UASRT
 * @param   baudrate 波特率
 * 
 * @return none
 */
void UART_Set_baudrate(UARTS UART_Select,u32 baudrate)
{
    USART_InitTypeDef USART_InitStructure = {0};

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    switch(UART_Select)
    {
        case UART1:
        {
            while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
            
            USART_Cmd(USART1, DISABLE);
            USART_Init(USART1, &USART_InitStructure);
            USART_Cmd(USART1, ENABLE);
            break;
        }
        case UART2:
        {
            while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET);

            USART_Cmd(USART2, DISABLE);
            USART_Init(USART2, &USART_InitStructure);
            USART_Cmd(USART2, ENABLE);
            break;
        }
        case UART3:
        {
            while(USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);

            USART_Cmd(USART3, DISABLE);
            USART_Init(USART3, &USART_InitStructure);
            USART_Cmd(USART3, ENABLE);
            break;
        }
        UART_Clear_Buffer(UART_Select);
    }
}

/**
 * @fn      UART_Set_Status
 *
 * @brief   更改UART状态
 *
 * @param   State 将要更新的状态
 * 
 * @return  none
 */
void UART_Set_Status(UARTS UART_Select, FunctionalState State)
{
    switch(UART_Select)
    {
        case UART1:
        {
            USART_Cmd(USART1, State);
            break;
        }
        case UART2:
        {
            USART_Cmd(USART2, State);
            break;
        }
        case UART3:
        {
            USART_Cmd(USART3, State);
            break;
        }
    }
}

void USART1_IRQHandler(void)
{
//    if((UART_Buffer1.End_Counter < UART_Buffer1.Start_Counter) && ((RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART1_RX_CH)) > UART_Buffer1.Start_Counter))
//        HardFault_Handler();
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData(USART1);
        UART_Buffer1.End_Counter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART1_RX_CH);
//        GPS_Location_Update();
    }

}

void USART2_IRQHandler(void)
{
//   if((UART_Buffer2.End_Counter < UART_Buffer2.Start_Counter) && ((RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART2_RX_CH)) > UART_Buffer2.Start_Counter))
//        HardFault_Handler();
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData(USART2);
        UART_Buffer2.End_Counter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART2_RX_CH);
        Location_Update();
    }
}

void USART3_IRQHandler(void)
{
//    if((UART_Buffer3.End_Counter < UART_Buffer3.Start_Counter) && ((RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART3_RX_CH)) > UART_Buffer3.Start_Counter))
//        HardFault_Handler();
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData(USART3);
        UART_Buffer3.End_Counter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART3_RX_CH);
    }
}

void DMA1_Channel5_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC5 | DMA1_IT_HT5);
//    if((UART_Buffer1.End_Counter < UART_Buffer1.Start_Counter) && ((RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART1_RX_CH)) > UART_Buffer1.Start_Counter))
//        HardFault_Handler();
    UART_Buffer1.End_Counter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART1_RX_CH);
}

void DMA1_Channel6_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC6 | DMA1_IT_HT6);
//    if((UART_Buffer2.End_Counter < UART_Buffer2.Start_Counter) && ((RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART2_RX_CH)) > UART_Buffer2.Start_Counter))
//        HardFault_Handler();
    UART_Buffer2.End_Counter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART2_RX_CH);
//    Location_Update();
}

void DMA1_Channel3_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC3 | DMA1_IT_HT3);
//    if((UART_Buffer3.End_Counter < UART_Buffer3.Start_Counter) && ((RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART3_RX_CH)) > UART_Buffer3.Start_Counter))
//        HardFault_Handler();
    UART_Buffer3.End_Counter = RX_BUFFER_LEN - DMA_GetCurrDataCounter(USART3_RX_CH);
}
