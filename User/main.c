/**
 * @file main.c
 * @author microgames (liusir26@outlook.com)
 * @brief 主函数
 * @date 2024-12-01
 * 
 * 
 */
#include "Motor.h"
#include "Route.h"
//#include "Ranging.h"
#include "Location.h"

int main(void)
{
    Motor_Init();
    Route_Init();
    LOCATION_Init();
    Motor_Set_PWM(50);
    Locations Location = LOCATION_Update();
    while(Location.X <= 10)
    {
        Location = LOCATION_Update();
    }
    Motor_Set_PWM(0);
}
