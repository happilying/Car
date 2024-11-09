#ifndef _GPS_H
#define _GPS_H

#include "USART.h"

#define GPS_UART UART3 // Assuming GPS is connected to UART2
#define GPS_BUFFER_SIZE 128
#define NMEA_SENTENCE_MAX_LENGTH 82
#define GPS_bool u32
#define GPS_MAX_SIZE 128

void GPS_Init(void);
void GPS_Get_Location();

#endif
