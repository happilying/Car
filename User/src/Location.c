#include "LOCATION.h"

Speeds Speed = {0};

void LOCATION_Init(void)
{
    IMU_Init();
}

void LOCATION_Update_Speed(u32 Start_Time)
{
    IMU_State IMU_Data = IMU_Get_Data();
    u32 Now_Time = LOCATION_Get_Time();
    Speed.VX = (Motor_Get_PWM() ? (Speed.VX + (Now_Time - Start_Time)/SystemCoreClock * IMU_Data.AX): 0);
    Speed.VY = (Motor_Get_PWM() ? (Speed.VX + (Now_Time - Start_Time)/SystemCoreClock * IMU_Data.AY): 0);
    Speed.RZ = IMU_Data.Z;
}