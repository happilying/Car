#ifndef _MOTOR_H
#define _MOTOR_H

#include "ch32v10x.h"

typedef enum {FORWARD = 0,BACKWARD = !FORWARD} DIRECTIONS;

#define MAX_PWM 100
#define MIN_PWM 20

#define MAX_SPEED 15.1
#define MIN_SPEED 2.1

#define A (MAX_PWM - MIN_PWM)/(MAX_SPEED - MIN_SPEED)



void Motor_Init(void);
void Motor_Set_PWM(u16 ccp);
u16 Motor_Get_PWM(void);
void Moter_Set_Direction(DIRECTIONS DIRECTION);
void Motor_Set_Speed(float SpeedM);
float Motor_Get_Speed(void);

#endif
