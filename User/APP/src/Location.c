#include "LOCATION.h"
#include <math.h>

Speeds Speed = {0};
u16 time = 0;
Locations Location = {0};
float A_X0 = 0,A_Y0 = 0;
u8 j = 0;

#ifdef USE_GPS
GPS_Location GPS_0 = {0};
#endif

extern void HardFault_Handler(void);

void Location_Init(void)
{
    IMU_Init();

    #ifdef USE_GPS
    GPS_Init();
    GPS_0 = GPS_Get_Location();
    #endif
}

void Location_Update(void)
{
    while(UART_Get_Length(IMU_UART) != 0)
    {
        IMU_State IMU_Data = IMU_Get_Data();
        if(IMU_Data.valid == 0)
            continue;
        float delta_t = (time <= IMU_Data.t_ms) ? (IMU_Data.t_ms - time) / 1000.0f : (1000 - time + IMU_Data.t_ms) / 1000.0f;
        if((delta_t > 0.01)&&j != 0)
        {
            DMA_Cmd(USART2_RX_CH,DISABLE);
            HardFault_Handler();
        }
        time = IMU_Data.t_ms;
        float A_X = -IMU_Data.AX * cosf(IMU_Data.Z * M_PI/180) - IMU_Data.AY * sinf(IMU_Data.Z * M_PI / 180);
        float A_Y = -IMU_Data.AY * cosf(IMU_Data.Z * M_PI/180) - IMU_Data.AX * sinf(IMU_Data.Z * M_PI / 180);
        Location.RZ = IMU_Data.Z;
        if(j == 0)
        {
            j = 1;
            break;
        }
        float V_X0 = Speed.VX;
        float V_Y0 = Speed.VY;
        Speed.VX = Speed.VX + delta_t * (A_X0 + A_X) / 2;
        Speed.VY = Speed.VX + delta_t * (A_Y0 + A_Y) / 2;
        Location.X = Location.X + delta_t * (V_X0 + Speed.VX) / 2;
        Location.Y = Location.Y + delta_t * (V_Y0 + Speed.VY) / 2;
        A_X0 = A_X;
        A_Y0 = A_Y;
    }

    #ifdef USE_GPS
    GPS_Location GPS = GPS_Location_Get();
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
}

Locations Location_Get(void)
{
    return Location;
}
