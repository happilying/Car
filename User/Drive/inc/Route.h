#ifndef _ROUTE_H
#define _ROUTE_H

#define MAX_ROTATION 195
#define MIN_ROTATION 110
#define CENTER 162

#include "ch32v10x.h"

void Route_Init(void);
void Route_Set_Angle(float Angles);
float Route_Get_Angle(void);


#endif
