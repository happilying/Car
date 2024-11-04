#ifndef _ROUTE_H
#define _ROUTE_H


#define MAX_ROTATION 175
#define MIN_ROTATION 75
#define CENTER (MAX_ROTATION+MIN_ROTATION)/2


#include "ch32v10x.h"


void Route_Init(void);
void Set_Angle(u16 Angle);
u16 Get_Angle(void);


#endif
