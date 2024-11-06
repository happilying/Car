#ifndef _USART_H
#define _USART_H


#include "ch32v10x.h"


#define USART1_RX_CH         DMA1_Channel5
#define USART2_RX_CH         DMA1_Channel6
#define USART3_RX_CH         DMA1_Channel3
#define RX_BUFFER_LEN        (2048u)


typedef enum {
    UART1 = 1,
    UART2,
    UART3
} UARTS;


typedef struct{
    volatile uint16_t Start_Conter;
    volatile uint16_t End_Conter;
    uint8_t RX_Buffer[RX_BUFFER_LEN];
} UART_Buffer;


void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));


void UART_Init(u32 baudrate,UARTS UART_Select);
void UART_Send_Data(UARTS UART_Select,u8 Data);
void UART_Clear_Buffer(UARTS UART_Select);
u8 UART_Get_Data(UARTS UART_Select);
u16 UART_Get_Length(UARTS UART_Select);
void UART_Send_Array(UARTS UART_Select,u8 *Array);


#endif
