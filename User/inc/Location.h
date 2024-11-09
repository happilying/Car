#ifndef _LOCATION_H
#define _LOCATION_H

#include "ch32v10x.h"
#include "IMU.h"
#include "GPS.h"
#include "Motor.h"

typedef struct
{
    float X,Y;
    float RZ;
} Locations;

typedef struct
{
    float VX,VY,RZ;
} Speeds;

void LOCATION_Init(void);
Locations LOCATION_Update(void);

#endif