#ifndef _IMU_H
#define _IMU_H

#include "USART.h"

#define ACC 0x51
#define ANG 0x53
#define TIME 0x50
#define IMU_UART UART2
#define IMU_Baudrate 9600
#define Noise_Count 100

typedef struct
{
   float AX,AY,Z;
   u16 t_ms;
} IMU_State;

u16 IMU_Init(void);
IMU_State IMU_Get_Data(void);

#endif
