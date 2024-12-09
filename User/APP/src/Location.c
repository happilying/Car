#include "LOCATION.h"
#include <math.h>

static Speeds Speed = {0};
static u16 time = 0;
static Locations Location = {0};
#ifndef _CODER_H
static float A_X0 = 0,A_Y0 = 0;
#endif
u8 IsFirst = 1;

#ifdef _GPS_H
static GPS_Location GPS_0 = {0};
#endif

#include <string.h>
#include <stdio.h>

#include "USART.h"

/**
 * @fn      Location_Init
 *
 * @brief   初始化Location应用
 *
 * @return none
 */
void Location_Init(void)
{
    UART_Init(UART1,115200);
    IMU_Init();

    #ifdef _GPS_H
    GPS_Init();
    while(GPS_0.latitude < 2 && GPS_0.longitude <2)
        GPS_0 = GPS_Location_Get();
    #endif

    #ifdef _CODER_H
    Coder_Init();
    #endif

    #ifdef _RANGING_H
    Ranging_Init();
    #endif
}

/**
 * @fn      Location_Update
 *
 * @brief   更新位置信息
 *
 * @return none
 */
void Location_Update(void)
{
    if(UART_Get_Length(IMU_UART) >= 33)
    {
        IMU_State IMU_Data = IMU_Get_Data();
        if (IMU_Data.valid == 2)
        {
            float delta_t = (time <= IMU_Data.t_ms) ? (IMU_Data.t_ms - time) / 1000.0f : (1000 - time + IMU_Data.t_ms) / 1000.0f;
            time = IMU_Data.t_ms;
            Location.RZ = IMU_Data.Z;
            float V_X0 = Speed.VX;
            float V_Y0 = Speed.VY;
            #ifndef _CODER_H
            if(IMU_Data.AX < 0.05 && IMU_Data.AX > -0.05)
                IMU_Data.AX = 0;
            if(IMU_Data.AY < 0.05 && IMU_Data.AY > -0.05)
                IMU_Data.AX = 0;
            float A_X = IMU_Data.AY * cosf(IMU_Data.Z * M_PI/180) + IMU_Data.AX * sinf(IMU_Data.Z * M_PI / 180);
            float A_Y = IMU_Data.AX * cosf(IMU_Data.Z * M_PI/180) - IMU_Data.AY * sinf(IMU_Data.Z * M_PI / 180);
            if(IsFirst)
            {
                A_X0 = A_X;
                A_Y0 = A_Y;
                IsFirst = 0;
            }
            else
            {
                Speed.VX = Speed.VX + delta_t * (A_X0 + A_X) / 2;
                Speed.VY = Speed.VY + delta_t * (A_Y0 + A_Y) / 2;
                A_X0 = A_X;
                A_Y0 = A_Y;
            #else
            if(IsFirst)
            {
                IsFirst = 0;
            }
            else
            {
                    float Speed0 = Coder_Get_Speed();
                    Speed.VX = Speed0 * cosf(IMU_Data.Z * M_PI/180);
                    Speed.VY = Speed0 * sinf(IMU_Data.Z * M_PI/180);
            #endif
                    Location.X = Location.X + delta_t * (V_X0 + Speed.VX) / 2;
                    Location.Y = Location.Y + delta_t * (V_Y0 + Speed.VY) / 2;
            #ifdef _RANGING_H
                    Ranging_Update();
            #endif
            //Distances ranging = Ranging_Get_Distance();
            char s[50];
            int len = sprintf(s,"%d,%d,%d\r\n",(int)(Location.X * 1000),(int)(Location.Y * 1000),(int)(Location.RZ * 10));
            UART_Send_Array(UART1, s, len);
            }
        }
    }

    #ifdef _GPS_H
    GPS_Location GPS = GPS_Location_Get();
    if(GPS.latitude > 2 && GPS.longitude > 2 && GPS_0.latitude >2 && GPS_0.longitude >2 && GPS.latitude < 40 && GPS.longitude < 180)
    {
        float GPS_X = (GPS.longitude - GPS_0.longitude) / M_LONG;
        float GPS_Y = (GPS.latitude - GPS_0.latitude) / M_LATI;
        if((GPS_X - Location.X >= 1)||(Location.X - GPS_X >= 1))
        {
            Location.X = (Location.X + GPS_X) / 2;
        }
        if((GPS_Y - Location.Y >= 1)|(Location.Y - GPS_Y >= 1))
        {
            Location.Y = (Location.Y + GPS_Y) / 2;
        }
    }
    #endif
}

/**
 * @fn      Location_Get
 *
 * @brief   获取当前位置信息
 * 
 * @return  Locations结构体
 */
Locations Location_Get(void)
{
    return Location;
}
