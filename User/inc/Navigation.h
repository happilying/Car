#ifndef _NAVIGATION_H
#define _NAVIGATION_H

#include "ch32v10x.h"
#include "IMU.h"
#include "GPS.h"

typedef struct
{
    uint16_t X,Y;
    int16_t RZ;
} Points;

typedef struct
{
    int16_t VX,VY
} Speeds;

void Navigation_Init(void);
void Navigation_Goto(Points Point);

#endif