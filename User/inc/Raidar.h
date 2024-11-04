#ifndef _RAIDAR_H
#define _RAIDAR_H

#include "USART.h"

typedef enum {
    Standard,
    Express,
    Boost,
    Sensitivity,
    Stability
} Modes;

void Raidar_Init(void);
void Raidar_Set_Rotation(u16 ccp);
u16 Raidar_Get_Rotation(void);
void Raidar_Start_Scan(Modes Mode);
void Raidar_Stop_Scan(void);


#endif
