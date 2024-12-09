#ifndef _TEST_H
#define _TEST_H

#define CAR_TO_FRONT_LENGTH 0.024
#define CAR_TO_BACK_LENGTH 0.008
#define CAR_WIDE_HALF 0.09
#define BASE_PWM 60

#include "Motor.h"
#include "Route.h"
#include "Location.h"
#include "USART.h"
#include "Light.h"
#include "Delay.h"

#include <string.h>
#include <stdio.h>

void test_begin(void);
void Forward_X(float X);
void Forward_Y(float Y);
void Route_to_angle(float angle);
void S_Y(float X,int cont);
void S_X(float Y,int cont);

#endif
