#ifndef _IMU_H
#define _IMU_H

#include "USART.h"

#define ACC 0x51
#define ANG 0x53
#define TIME 0x50
#define IMU_UART UART2
#define IMU_Baudrate 115200

typedef struct
{
   float AX,AY,Z;
   u16 t_ms;
   u8 valid;
} IMU_State;

void IMU_Init(void);
IMU_State IMU_Get_Data(void);

#endif
