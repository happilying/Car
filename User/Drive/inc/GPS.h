#ifndef _GPS_H
#define _GPS_H

#include "USART.h"

#define GPS_UART UART1 // Assuming GPS is connected to UART2
#define GPS_BUFFER_SIZE 128
#define NMEA_SENTENCE_MAX_LENGTH 82
#define GPS_bool u32
#define GPS_MAX_SIZE 128
#define GPS_baud 9600

/**
 * @brief 
 * 
 */
typedef struct
{
    float latitude,///<纬度
    longitude;///<经度
} GPS_Location;

void GPS_Init(void);
GPS_Location GPS_Get_Location(void);

#endif
