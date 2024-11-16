#include "LOCATION.h"
#include <math.h>

Speeds Speed = {0};
u16 time = 0;
Locations Location = {0};
float A_X0 = 0,A_Y0 = 0;

#ifdef USE_GPS
GPS_Location GPS_0 = {0};
#endif

void LOCATION_Init(void)
{
    IMU_State IMU_Data ={0};

    IMU_Init();
    IMU_Data = IMU_Get_Data();
    time = IMU_Data.t_ms;

    #ifdef USE_GPS
    GPS_Init();
    GPS_0 = GPS_Get_Location();
    #endif
}

Locations LOCATION_Update(void)
{
    IMU_State IMU_Data = IMU_Get_Data();
    while((IMU_Data.AX != 0)|(IMU_Data.AY != 0)|(IMU_Data.Z != 0)|(IMU_Data.t_ms != 0))
    {
        float V_X0 = Speed.VX;
        float V_Y0 = Speed.VY;
        float A_X = -IMU_Data.AX * cosf(IMU_Data.Z * M_PI/180) - IMU_Data.AY * sinf(IMU_Data.Z * M_PI / 180);
        float A_Y = -IMU_Data.AY * cosf(IMU_Data.Z * M_PI/180) - IMU_Data.AX * sinf(IMU_Data.Z * M_PI / 180);
        float delta_t = (time >= IMU_Data.t_ms) ? (time - IMU_Data.t_ms) / 1000 : (time + 65536 - IMU_Data.t_ms) / 1000;
        Speed.VX = Speed.VX + delta_t * (A_X0 + A_X) / 2;
        Speed.VY = Speed.VX + delta_t * (A_Y0 + A_Y) / 2;
        Location.X = Location.X + delta_t * (V_X0 + Speed.VX) / 2;
        Location.Y = Location.Y + delta_t * (V_Y0 + Speed.VY) / 2;
        Location.RZ = IMU_Data.Z;

        time = IMU_Data.t_ms;
        A_X0 = A_X;
        A_Y0 = A_Y;

        IMU_Data = IMU_Get_Data();
    }

    #ifdef USE_GPS
    GPS_Location GPS = GPS_Get_Location();
    float GPS_X = (GPS.longitude - GPS_0.longitude) / M_LONG;
    float GPS_Y = (GPS.latitude - GPS_0.latitude) / M_LATI;
    if((GPS_X - Location.X >= 5)|(Location.X - GPS_X >= 5))
    {
        Location.X = GPS_X;
    }
    if((GPS_Y - Location.Y >= 5)|(Location.Y - GPS_Y >= 5))
    {
        Location.Y = GPS_Y;
    }
    #endif
    return Location;
}

Speeds Speed_Update(void)
{
    LOCATION_Update();
    return Speed;
}
