#ifndef _DELAY_H
#define _DELAY_H

#include "ch32v10x.h"

void Delay_Init(void);
void Delay_Us(uint32_t n);
void Delay_Ms(uint32_t n);

#endif