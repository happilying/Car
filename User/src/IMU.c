#include "IMU.h"
#include "Command_IMU.h"

void IMU_Init(void)
{
    UART_Init(9600, UART2);
}

void IMU_Get_Data(IMU_State *IMU)
{
    u8 buffer[10] = {0};
    u8 Check = 0;
    for(u8 i = 0;i <= (sizeof(Commander) - 1);i++)
    {
        UART_Send_Data(UART3,Commander[i]);
    }
    while(UART_Get_Length(UART3))
    {
        for(u8 i =0;i <= (sizeof(buffer) - 1);i++)
        {
            buffer[i] = UART_Get_Data(UART3);
            Check += buffer[i];
        }
        if(Check == UART_Get_Data(UART3))
        {
            switch(buffer[1])
            {
                case ACC:
                {
                    IMU->AX = ((int16_t)((buffer[2] << 8)|buffer[3]) / 32768 * 16 * 9.8);
                    IMU->AY = ((int16_t)((buffer[4] << 8)|buffer[5]) / 32768 * 16 * 9.8);
                    break;
                }
                case ANG:
                {
                    IMU->Z = ((int16_t)((buffer[6] << 8)|buffer[7]) / 32768 * 180);
                }
            }
        }
    }
}