#include "Ranging.h"

void Ranging_Init(void)
{
    UART_Init(Ranging_Baudrate,Ranging_UART);
}

Distances Ranging_Get_Distance(void)
{
    Distances Distance = {0};
    return Distance;
}
