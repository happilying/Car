/**
 * @file main.c
 * @author microgames (liusir26@outlook.com)
 * @brief 涓诲嚱鏁�
 * @date 2024-12-01
 * 
 * 
 */
#include "Motor.h"
//#include "Route.h"
//#include "Ranging.h"
//#include "Location.h"
//#include "USART.h"
#include "GPS.h"

//#include <string.h>

int main(void)
{
//    Location_Init();
//    Motor_Init();
//    UART_Init(UART1, 115200);
//    UART_Send_Array(UART1, "Start OK!\r\n", strlen("Start OK!\r\n"));
//    while(UART_Get_Data(UART1) == 0);
//    UART_Send_Array(UART1, "Start!\r\n", strlen("Start!\r\n"));
//    Motor_Set_PWM(50);
//    Locations Location = {0};
//    while(Location.X <= 0.5)
//    {
//        Location = Location_Get();
//    }
//    Motor_Set_PWM(0);
//    UART_Send_Array(UART1, "Stop!\r\n", strlen("Stop!\r\n"));
    GPS_Init();
    while(1)
    {
        volatile GPS_Location GPS = GPS_Location_Get();
    }
}
