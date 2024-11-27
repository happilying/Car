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

int main(void)
{
   Location_Init();
   Motor_Init();
//   Motor_Set_PWM(30);
   Locations Location12 = {0};
   while(Location12.X <= 0.5 && Location12.X >= -0.5)
   {
       Location12 = Location_Get();
       Delay_Ms(500);
   }
//   Motor_Set_PWM(0);
   while(1);
    // GPS_Init();
    // while(1)
    // {
    //     volatile GPS_Location GPS = GPS_Location_Get();
    // }
}
