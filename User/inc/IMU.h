#ifndef _IMU_H
#define _IMU_H

#include "USART.h"

#define ACC (0x51)
#define ANG (0x53)

typedef struct
{
   int16_t AX,AY,Z;
}IMU_State;

void IMU_Init(void);
void IMU_Get_Data(IMU_State *IMU);

#endif
