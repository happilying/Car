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
#include "Location.h"
#include "USART.h"
// #include "GPS.h"

#include <string.h>
#include <stdio.h>

int main(void)
{
   Location_Init();
   Motor_Init();
   UART_Init(UART1,115200);
//   Motor_Set_PWM(30);
   volatile Locations Location12 = {0};
   while(Location12.X <= 0.5 && Location12.X >= -0.5)
   {
       char s[50];
       Location12 = Location_Get();
       int len = sprintf(s,"%d,%d\r\n",Location12.X * 100,Location12.Y * 100);
       UART_Send_Array(UART1, s, len);
   }
   Motor_Set_PWM(0);
    // GPS_Init();
    // while(1)
    // {
    //     volatile GPS_Location GPS = GPS_Location_Get();
    // }
}
