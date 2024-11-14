#include "GPS.h"
#include "USART.h"
#include <string.h>
#include <stdlib.h>

/**
 * @brief 获取GPS波特率消息
 * @param baud 要设置的波特率
 * @return 对应波特率的NMEA字符串
 */
char *GPS_getBaudRateMessage(int baud)
{
    switch (baud)
    {
    case 4800:
        return "$PMTK251,4800*14\r\n";
    case 9600:
        return "$PMTK251,9600*17\r\n";
    case 14400:
        return "$PMTK251,14400*29\r\n";
    case 19200:
        return "$PMTK251,19200*22\r\n";
    case 38400:
        return "$PMTK251,38400*27\r\n";
    case 57600:
        return "$PMTK251,57600*2C\r\n";
    case 115200:
        return "$PMTK251,115200*1F\r\n";
    default:
        return NULL;
    }
}

u8 GPS_getChecksum(char *message)
{
    char *temp = message + 1; // Skip the $
    u8 checksum = 0;
    while (*temp != '*')
    {
        checksum ^= *temp;
        temp++;
    }
    return checksum;
}

/**
 * @brief 设置GPS模式
 * @param GGA GGA模式
 * @param GLL GLL模式
 * @param GSA GSA模式
 * @param GSV GSV模式
 * @param RMC RMC模式
 * @param VTG VTG模式
 * @return 成功返回1
 */
int GPS_setModes(u8 GGA, u8 GLL, u8 GSA, u8 GSV, u8 RMC, u8 VTG)
{
    char message[52] = {0};
    u8 checksum;

    sprintf(message, "$PMTK314,%d,%d,%d,%d,%d,%d,0,0,0,0,0,0,0,0,0,0,0,0,0*",
            GLL, RMC, VTG, GGA, GSA, GSV);

    checksum = GPS_getChecksum(message);
    sprintf(message,
            "$PMTK314,%d,%d,%d,%d,%d,%d,0,0,0,0,0,0,0,0,0,0,0,0,0*%X\r\n", GLL,
            RMC, VTG, GGA, GSA, GSV, checksum);

    if (*message)
        UART_Send_Array(GPS_UART, (u8 *)message, sizeof(message));

    return 1;
}

/**
 * @brief 设置GPS通信的波特率
 * @param boundrate 期望的波特率
 */
void GPS_Set_BoundRate(int boundrate)
{
    char *message = GPS_getBaudRateMessage(boundrate);
    UART_Send_Array(GPS_UART, (u8 *)message, sizeof(message));
}

/**
 * @brief 初始化GPS通信
 */
void GPS_Init(void)
{
    // 初始化UART用于GPS通信
    UART_Init(GPS_UART, GPS_baud);

    // 配置GPS模块仅ping RMC
    GPS_setModes(0, 0, 0, 0, 1, 0);
    Delay_Ms(2 * 1000);

    // 清空GPS缓冲区
    UART_Clear_Buffer(GPS_UART);
}

/**
 * @brief 从GPS获取位置
 * @param latitude 存储纬度的指针
 * @param longitude 存储经度的指针
 */
GPS_Location GPS_Get_Location(void)
{
    char *token;
    char nmeaSentence[NMEA_SENTENCE_MAX_LENGTH];
    uint16_t i = 0;
    GPS_Location Location = {0};
    // 等待完整的NMEA语句
    while (1)
    {
        if (UART_Get_Length(GPS_UART) > 0)
        {
            char c = UART_Get_Data(GPS_UART);

            if (c == '\n' && i > 0 && nmeaSentence[i - 1] == '\r')
            {
                // 接收到完整的NMEA语句
                nmeaSentence[i] = '\0';
                break;
            }
            else if (i < NMEA_SENTENCE_MAX_LENGTH - 1)
            {
                nmeaSentence[i++] = c;
            }
        }
    }

    // 解析GPRMC语句以获取位置数据
    if (strstr(nmeaSentence, "$GPRMC") != NULL)
    {
        token = strtok(nmeaSentence, ",");

        // 忽略前3个标记
        for (int i = 0; i < 3; i++)
        {
            token = strtok(NULL, ",");
        }

        // 提取纬度
        if (token != NULL)
        {
            Location.latitude = atof(token) / 100.0;
            token = strtok(NULL, ",");
            if (token != NULL && token[0] == 'S')
            {
                Location.latitude = -Location.latitude;
            }
        }

        // 提取经度
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            Location.longitude = atof(token) / 100.0;
            token = strtok(NULL, ",");
            if (token != NULL && token[0] == 'W')
            {
                Location.longitude = -Location.longitude;
            }
        }
    }
}
