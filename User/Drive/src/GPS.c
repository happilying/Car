#include "GPS.h"
#include "USART.h"
#include <string.h>
#include <stdlib.h>

GPS_Location LocationG = {0};

/**
 * @brief 初始化GPS通信
 */
void GPS_Init(void)
{
    // 初始化UART用于GPS通信
    UART_Init(GPS_UART, GPS_baud);
}

/**
 * @brief 从GPS获取位置
 * 
 * @return GPS_Location结构体
 */
void GPS_Location_Update(void)
{
    char *token;
    char nmeaSentence[NMEA_SENTENCE_MAX_LENGTH] = {0};
    uint16_t i = 0;

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
    if (strstr(nmeaSentence, "$GNRMC") != NULL)
    {
        token = strtok(nmeaSentence, ",");

        // 忽略前2个标记
        for (int i = 0; i < 2; i++)
        {
            token = strtok(NULL, ",");
        }
        if(strncmp(token,"V",1))
        {
            token = strtok(NULL, ",");
            // 提取纬度
            if (token != NULL)
            {
                char dd[2] = {*token, *(token + 1)};
                LocationG.latitude = atoi(dd) + atof(token + 2) / 60.0;
                token = strtok(NULL, ",");
                if (token != NULL && token[0] == 'S')
                {
                    LocationG.latitude = -LocationG.latitude;
                }
            }

            // 提取经度
            token = strtok(NULL, ",");
            if (token != NULL)
            {
                char dd[3] = {*token, *(token + 1), *(token + 2)};
                LocationG.longitude = atoi(dd) + atof(token + 3) / 60.0;
                token = strtok(NULL, ",");
            }
            if (token != NULL && token[0] == 'W')
            {
                LocationG.longitude = -LocationG.longitude;
            }
        }
        else
        {
            LocationG.latitude = 1;
            LocationG.longitude = 1;
        }
    }
}

GPS_Location GPS_Location_Get(void)
{
    return LocationG;
}
