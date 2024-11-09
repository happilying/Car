#include "LOCATION.h"

Speeds Speed = {0};
u16 time = 0;
Locations Location = {0};

void LOCATION_Init(void)
{
    time = IMU_Init();
}

Locations LOCATION_Update(void)
{
    IMU_State IMU_Data = IMU_Get_Data();
    float delta_t = (time >= IMU_Data.t_ms) ? (time - IMU_Data.t_ms)/1000 : (time + 65536 - IMU_Data.t_ms)/1000;
    Speed.VX = (Motor_Get_PWM() ? (Speed.VX + delta_t * IMU_Data.AX): 0);
    Speed.VY = (Motor_Get_PWM() ? (Speed.VX + delta_t * IMU_Data.AY): 0);
    Speed.RZ = IMU_Data.Z;
}