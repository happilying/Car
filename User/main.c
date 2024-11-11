#include "Motor.h"
#include "Route.h"
#include "USART.h"
//#include "Raidar.h"
//#include "Ranging.h"
#include "IMU.h"

//u32 count = 0;
int main(void)
{
    IMU_Init();
    IMU_State IMU = IMU_Get_Data();
    while(1);
}
