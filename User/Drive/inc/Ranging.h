#ifndef _RANGING_H
#define _RANGING_H

#include "USART.h"

#define Ranging_UART UART3
#define Ranging_Baudrate 115200
#define BUFFER_SIZE 32
#define START_BYTE 0xFF
#define END_BYTE 0xFE
#define CRC_POLY 0xA001

typedef enum
{
    RANGING_COMMAND_READ = 0x03, // 0x03 璇�
    RANGING_COMMAND_WRITE = 0x06 // 0x06 鍐�
} Ranging_Command;

typedef enum
{
    RANGING_REGISTER_DISTANCE = 0x34,
    RANGING_REGISTER_STATUS = 0x35
} Ranging_Register;

typedef enum
{
    LEFT_DEVICE_ID = 0x50,
    RIGHT_DEVICE_ID = 0x51
} Device_ID;

typedef struct
{
    u16 Left,right;
} Distances;

void Ranging_Init(void);
void Ranging_Update(void);
Distances Ranging_Get_Distance(void);

#endif
