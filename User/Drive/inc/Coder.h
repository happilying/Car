#ifndef _CODER_H
#define _CODER_H

#include "ch32v10x.h"

#define WHEEL_R 0.0325
#define DELTA 1.22

void TIM4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void Coder_Init(void);
float Coder_Get_Speed(void);

#endif
