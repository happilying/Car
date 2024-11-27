#ifndef _LOCATION_H
#define _LOCATION_H

#include "IMU.h"

#define M_LONG 0.00001174
#define M_LATI 0.00000899

//如果使用GPS，取消注释
//#define USE_GPS

#ifdef USE_GPS
#include "GPS.h"
#endif

/**
 * @struct
 * @brief 
 * 
 */
typedef struct
{
    float X,Y,RZ,dt;
} Locations;

typedef struct
{
    float VX,VY;
} Speeds;

void Location_Init(void);
void Location_Update(void);
Locations Location_Get(void);

#endif
