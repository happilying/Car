#ifndef _RANGING_H
#define _RANGING_H

#include "Location.h"

#define Ranging_UART UART1
#define Ranging_Baudrate 115200
#define BUFFER_SIZE 32
#define START_BYTE 0xFF
#define END_BYTE 0xFE
#define CRC_POLY 0xA001

typedef enum
{
    RANGING_COMMAND_READ = 0x03, // 0x03 读
    RANGING_COMMAND_WRITE = 0x06 // 0x06 写
} Ranging_Command;

typedef enum
{
    RANGING_REGISTER_DISTANCE = 0x60,
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
Distances Ranging_Get_Distance(void);
uint16_t Calculate_CRC(uint8_t *data, uint8_t length);
char *Ranging_CreateReadDistanceMessage(uint8_t device_id);
int Ranging_VerifyReadDistanceResponse(uint8_t device_id);

#endif
