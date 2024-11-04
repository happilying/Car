#include "Motor.h"
#include "Route.h"
#include "USART.h"
#include "Raidar.h"

//u32 count = 0;

int main(void)
{
    Motor_Init();
    Route_Init();
    Raidar_Init();
    Raidar_Set_Rotation(100);
    UART_Init(115200, UART1);
    UART_Send_Data(UART1, 0x66);
    Raidar_Start_Scan(Standard);
    while(1)
    {
        UART_Send_Data(UART1, UART_Get_Data(UART3));
    }
/*    for(int i = 0;i < 7;i++)
    {
        UART_Send_Data(UART1, UART_Get_Data(UART3));
    }
    UART_Get_Data(UART3);
    count++;
    while(!(UART_Get_Data(UART3) & 1))
    {
        for(int i = 0;i<4;i++)
        {
            UART_Send_Data(UART1, UART_Get_Data(UART3));
        }
        count++;
    }
    while(1); */
}
