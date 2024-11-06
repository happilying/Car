#include "IMU.h"
#include "Command_IMU.h"
#include "Delay.h"

IMU_State Noise = {0};

void IMU_Init(void)
{
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
    u8 buffer[10] = {0};
    u8 Check = 0;
    UART_Send_Array(IMU_UART,Commander);
    while(!UART_Get_Length(IMU_UART))
    {
        for(u8 i =0;i <= (sizeof(buffer) - 1);i++)
        {
            buffer[i] = UART_Get_Data(IMU_UART);
            Check += buffer[i];
        }
        if(Check == UART_Get_Data(IMU_UART))
        {
            switch(buffer[1])
            {
                case ACC:
                {
                    IMU.AX = ((int32_t)((buffer[2] << 8)|buffer[3]) / 32768 * 16 * 9800000 - Noise.AX);
                    IMU.AY = ((int32_t)((buffer[4] << 8)|buffer[5]) / 32768 * 16 * 9800000 - Noise.AY);
                    break;
                }
                case ANG:
                {
                    IMU.Z = ((int32_t)((buffer[6] << 8)|buffer[7]) / 32768 * 180000 - Noise.Z);
                }
            }
        }
    }
    return IMU;
}