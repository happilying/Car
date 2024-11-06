#include "Motor.h"
#include "Route.h"
#include "USART.h"
//#include "Raidar.h"
#include "Ranging.h"

//u32 count = 0;
IMU_State IMU1 = {0};
int main(void)
{
    Motor_Init();
    Route_Init();
    UART_Init(115200, UART1);
    IMU_Init();
    UART_Send_Data(UART1, 0x66);
    while(1)
    {
        UART_Send_Data(UART1, UART_Get_Data(UART2));
    }
}
