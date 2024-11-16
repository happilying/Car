#ifndef _LOCATION_H
#define _LOCATION_H

#include "IMU.h"
#include "GPS.h"
#include "Motor.h"

#define M_LONG 0.00001174
#define M_LATI 0.00000899

//如果使用GPS，取消注释
//#define USE_GPS

/**
 * @struct
 * @brief 
 * 
 */
typedef struct
{
    float X,Y,RZ;
} Locations;

typedef struct
{
    float VX,VY;
} Speeds;

void LOCATION_Init(void);
Locations LOCATION_Update(void);
Speeds Speed_Update(void);

#endif