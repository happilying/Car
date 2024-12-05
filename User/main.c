/**
 * @file main.c
 * @author microgames (liusir26@outlook.com)
 * @brief 涓诲嚱鏁�
 * @date 2024-12-01
 * 
 * 
 */
#include "Motor.h"
#include "Route.h"
#include "Location.h"

// #include <string.h>
// #include <stdio.h>

int main(void)
{

   Location_Init();
   Motor_Init();
   Route_Init();
   Motor_Set_PWM(40);
   Locations Location1 = {0};
   while(Location1.X <= 5 && Location1.X >= -5)
   {
      Location1 = Location_Get();
      char s[50];
      int len = sprintf(s,"%d\r\n",(int)(Location1.X * 1000));
      UART_Send_Array(UART3, s, len);
      Delay_Ms(500);
   }
    // GPS_Init();
    // while(1)
    // {
    //     volatile GPS_Location GPS = GPS_Location_Get();
    // }
   Motor_Set_PWM(0);
}
