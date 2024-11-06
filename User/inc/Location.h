#ifndef _LOCATION_H
#define _LOCATION_H

#include "ch32v10x.h"
#include "IMU.h"
#include "GPS.h"
#include "Motor.h"

typedef struct
{
    uint16_t X,Y;
    int16_t RZ;
} Locations;

typedef struct
{
    int16_t VX,VY,RZ;
} Speeds;

void LOCATION_Init(void);
void LOCATION_Update_Speed(u32 Start_Time);
u32 LOCATION_Get_Time(void);


#endif