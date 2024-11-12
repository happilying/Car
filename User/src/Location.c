#include "LOCATION.h"
#include <math.h>

Speeds Speed = {0};
u16 time = 0;
Locations Location = {0};

void LOCATION_Init(void)
{
    GPS_Location GPS = {0};
    IMU_Init();
    GPS_Init();
    GPS = GPS_Get_Location();
}

Locations LOCATION_Update(void)
{
    IMU_State IMU_Data = IMU_Get_Data();
    while((IMU_Data.AX != 0)|(IMU_Data.AY != 0)|(IMU_Data.Z != 0)|(IMU_Data.t_ms != 0))
    {
        float delta_t = (time >= IMU_Data.t_ms) ? (time - IMU_Data.t_ms)/1000 : (time + 65536 - IMU_Data.t_ms)/1000;
 //       float A_X = -a_x*cosf(Car_inst.angle*PI/180)-a_y*sinf(Car_inst.angle*PI/180);
        Speed.VX = (Motor_Get_PWM() ? (Speed.VX + delta_t * IMU_Data.AX): 0);
        Speed.VY = (Motor_Get_PWM() ? (Speed.VX + delta_t * IMU_Data.AY): 0);
        Speed.RZ = IMU_Data.Z;
        time = IMU_Data.t_ms;
        IMU_Data = IMU_Get_Data();
    }
    return Location;
}