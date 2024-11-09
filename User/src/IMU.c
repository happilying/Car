#include "IMU.h"
#include "Command_IMU.h"
#include "Delay.h"

IMU_State Noise = {0};

u16 IMU_Init(void)
{
    u16 Start_time;
    UART_Init(IMU_Baudrate, IMU_UART);
    UART_Send_Array(IMU_UART,Unlock);
    Delay_Ms(190);
    UART_Send_Array(IMU_UART,Z_Calibration);
    Delay_Ms(3000);
    UART_Send_Array(IMU_UART,Save);
    for(u8 i = 0;i <= Noise_Count - 1;i++)
    {
        IMU_State IMU = IMU_Get_Data();
        Noise.AX += IMU.AX;
        Noise.AY += IMU.AY;
        Noise.Z += IMU.Z;
    }
    Noise.AX /= Noise_Count;
    Noise.AY /= Noise_Count;
    Noise.Z /= Noise_Count;
}

IMU_State IMU_Get_Data(void)
{
    IMU_State IMU;
    u8 buffer[9] = {0};
    u8 Check = 0x55;
    while(!UART_Get_Length(IMU_UART));
    for(u8 j = 0;j <= 2;j++)
    {
        while(UART_Get_Data(IMU_UART) == 0x55);
        for(u8 i = 0;i <= (sizeof(buffer) - 1);i++)
        {
            buffer[i] = UART_Get_Data(IMU_UART);
            Check += buffer[i];
        }
        if(Check == UART_Get_Data(IMU_UART))
        {
            switch(buffer[0])
            {
                case ACC:
                {
                    IMU.AX = ((int32_t)((buffer[1] << 8)|buffer[2]) / 32768 * 16 * 9800000 - Noise.AX);
                    IMU.AY = ((int32_t)((buffer[3] << 8)|buffer[4]) / 32768 * 16 * 9800000 - Noise.AY);
                    break;
                }
                case ANG:
                {
                    IMU.Z = ((int32_t)((buffer[5] << 8)|buffer[6]) / 32768 * 180000 - Noise.Z);
                    break;
                }
                case TIME:
                {
                    IMU.t_ms = (u16)((buffer[7] << 8)|buffer[8]);
                    break;
                }
                default:break;
            }
        }
    }
    return IMU;
}