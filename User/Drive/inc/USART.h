#ifndef _USART_H
#define _USART_H

#include "ch32v10x.h"

#define USART1_RX_CH         DMA1_Channel5
#define USART2_RX_CH         DMA1_Channel6
#define USART3_RX_CH         DMA1_Channel3
#define RX_BUFFER_LEN        (256u)

typedef enum {
    UART1 = 1,
    UART2,
    UART3
} UARTS;

typedef struct{
    volatile uint8_t Start_Counter;
    volatile uint8_t End_Counter;
    uint8_t RX_Buffer[RX_BUFFER_LEN];
} UART_Buffer;

void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void UART_Init(UARTS UART_Select,u32 baudrate);
void UART_Send_Data(UARTS UART_Select,u8 Data);
void UART_Clear_Buffer(UARTS UART_Select);
u8 UART_Get_Data(UARTS UART_Select);
u8 UART_Get_Length(UARTS UART_Select);
void UART_Send_Array(UARTS UART_Select,u8 *Array,int Length);
void UART_Set_baudrate(UARTS UART_Select,u32 baudrate);
u8 UART_Get_Data_With_Position(UARTS UART_Select, int position);
void UART_Set_Status(UARTS UART_Select, FunctionalState State);

#endif
