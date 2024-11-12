#include "Motor.h"
#include "Route.h"
#include "USART.h"
//#include "Raidar.h"
//#include "Ranging.h"
#include "IMU.h"

//u32 count = 0;
int main(void)
{
    volatile IMU_State Noise = {0},IMU1 = {0},IMU2 = {0};
    Noise = IMU_Init();
    while(IMU1.t_ms == 0)
        IMU1 = IMU_Get_Data();
    while(IMU2.t_ms == 0)
        IMU2 = IMU_Get_Data();
    while(1);
}
