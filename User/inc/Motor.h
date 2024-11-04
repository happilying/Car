#ifndef _MOTOR_H
#define _MOTOR_H


#include "ch32v10x.h"


typedef enum {FORWARD = 0,BACKWARD = !FORWARD} DIRECTIONS;


void Motor_Init(void);
void Motor_Set_PWM(u16 ccp);
u16 Moter_Get_PWM(void);
void Moter_Set_Direction(DIRECTIONS DIRECTION);


#endif
