#ifndef _RANGING_H
#define _RANGING_H

#include "Location.h"

#define Ranging_UART UART1
#define Ranging_Baudrate 115200

typedef struct
{
    u16 Left,right;
} Distances;

void Ranging_Init(void);
Distances Ranging_Get_Distance(void);

#endif