#include "Ranging.h"
#include <string.h>

void Ranging_Init(void)
{
    UART_Init(Ranging_UART, Ranging_Baudrate);
}

uint16_t Calculate_CRC(uint8_t *data, uint8_t length)
{
    uint16_t crc = 0xFFFF;

    for (uint8_t i = 0; i < length; i++)
    {
        crc ^= (uint16_t)data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc = (crc >> 1) ^ CRC_POLY;
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }

    return crc;
}

// 例子
// 发：50 03 00 34 00 01 C8 45
// 50: 设备ID
// 03: 读模式
// 00 34: 寄存器地址
// 00 01: 寄存器数量
// C8 45: CRC校验
char *Ranging_CreateReadDistanceMessage(uint8_t device_id)
{
    static char message[8] = {0};
    uint16_t crc = 0;

    message[0] = device_id;
    message[1] = RANGING_COMMAND_READ;
    message[2] = 0x00;
    message[3] = RANGING_REGISTER_DISTANCE;
    message[4] = 0x00;
    message[5] = 0x01;

    crc = Calculate_CRC((uint8_t *)message, 6);
    message[6] = crc & 0xFF;
    message[7] = (crc >> 8) & 0xFF;

    return message;
}

// 例子
// 接受：50 03 02 07 0B 06 7F
// 50: 命令ID
// 03: 设备ID
// 02: 读模式
// 07 0B: 数据
// 06 7F: CRC校验
int Ranging_VerifyReadDistanceResponse(uint8_t device_id)
{
    // 确认设备ID
    if (UART_Get_Data_With_Position(Ranging_UART, 0) != device_id)
        return 0;

    // TODOS: add CRC verification
    // uint16_t crc = Calculate_CRC((uint8_t *)response, 6);
    // if (response[6] == (crc >> 8) && response[7] == (crc & 0xFF))
    // {
    return 1;
    // }
}

/**
 * @brief 
 * 
 * @return Distances 
 */
Distances Ranging_Get_Distance(void)
{
    Distances result = {0, 0};

    char *readMessageLeft = Ranging_CreateReadDistanceMessage(LEFT_DEVICE_ID);
    char *readMessageRight = Ranging_CreateReadDistanceMessage(RIGHT_DEVICE_ID);

    //
    //
    // Process the left device
    if (*readMessageLeft)
        UART_Send_Array(Ranging_UART, (u8 *)readMessageLeft, 8);

    while (Ranging_VerifyReadDistanceResponse(LEFT_DEVICE_ID) == 0)
        ;

    if (Ranging_VerifyReadDistanceResponse(LEFT_DEVICE_ID))
    {
        // Extract the distance value
        uint16_t distance = (UART_Get_Data_With_Position(Ranging_UART, 3) << 8) | UART_Get_Data_With_Position(Ranging_UART, 4);
        result.Left = distance;
    }

    //
    //
    // Process the right device
    if (*readMessageRight)
        UART_Send_Array(Ranging_UART, (u8 *)readMessageRight, sizeof(readMessageRight));

    while (Ranging_VerifyReadDistanceResponse(RIGHT_DEVICE_ID) == 0)
        ;

    // Extract the distance value
    uint16_t distance = (UART_Get_Data_With_Position(Ranging_UART, 3) << 8) | UART_Get_Data_With_Position(Ranging_UART, 4);
    result.Left = distance;

    // TODO: Verify the state

    return result;
}
