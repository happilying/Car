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
   float AX,AY,Z;//X,Y加速度与Z旋转
   u16 t_ms;//时间戳
   u8 valid;//包有效表示，0为无效
} IMU_State;

void IMU_Init(void);
IMU_State IMU_Get_Data(void);

#endif
