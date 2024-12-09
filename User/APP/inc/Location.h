#ifndef _LOCATION_H
#define _LOCATION_H

#include "IMU.h"
#include "Coder.h"
//#include "Ranging.h"
//#include "GPS.h"

#define M_LONG 0.00001174
#define M_LATI 0.00000899



/**
 * @struct
 * @brief 
 * 
 */
typedef struct
{
    float X,Y,RZ;//X位置（前方），Y位置，旋转（顺时针为正）。
} Locations;

typedef struct
{
    float VX,VY;
} Speeds;

void Location_Init(void);
void Location_Update(void);
Locations Location_Get(void);

#endif
