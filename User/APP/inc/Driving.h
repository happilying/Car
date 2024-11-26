#ifndef _DRIVING_H
#define _DRIVING_H

#include "stdint.h"

// 函数原型
void stop_car(void);
void move_forward(void);
void drive_s(uint8_t driving_s);
void drive_3s(void);
void drive_500ms(void);
void align_at_door(int areObstaclesOnBothSides);
void navigation_to_next_checkpoint(int areObstaclesOnBothSides);
void adjust_angle_for_circular_path(void);

#endif