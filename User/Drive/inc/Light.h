#ifndef _LIGHT_H
#define _LIGHT_H

#include "ch32v10x.h"

typedef enum {ON = 1,OFF = !ON} Status;

void Light_Init(void);
void Light_Set_Status(Status state);

#endif