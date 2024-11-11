#include "IMU.h"
#include "Command_IMU.h"
#include "Delay.h"

IMU_State Noise = {0};

/**
 * @fn      IMU_Init
 *
 * @brief   初始化IMU模块，并校准Z旋转
 * 
 * @return  IMU最后汇报时间（u16），
 */
u16 IMU_Init(void)
{
    u16 Start_time = 0;
    UART_Init(IMU_Baudrate, IMU_UART);
    UART_Send_Array(IMU_UART,Unlock,sizeof(Unlock));
    Delay_Ms(190);
    UART_Send_Array(IMU_UART,Z_Calibration,sizeof(Z_Calibration));
    Delay_Ms(3000);
    UART_Send_Array(IMU_UART,Save,sizeof(Save));
    for(u8 i = 0;i <= Noise_Count - 1;i++)
    {
        IMU_State IMU = IMU_Get_Data();
        Noise.AX += IMU.AX;
        Noise.AY += IMU.AY;
        Noise.Z += IMU.Z;
        if(i == Noise_Count - 1)
        {
            Start_time = IMU.t_ms;
        }
    }
    Noise.AX /= Noise_Count;
    Noise.AY /= Noise_Count;
    Noise.Z /= Noise_Count;
    return Start_time;
}

IMU_State IMU_Get_Data(void)
{
    IMU_State IMU = {0};
    u8 buffer[9] = {0};
    u8 Check = 0x55;
    UART_Send_Array(IMU_UART, Commander, sizeof(Commander));
    Delay_Ms(1);
    if(!UART_Get_Length(IMU_UART))
    {
        return IMU;
    }
    for(u8 j = 0;j <= 3;j++)
    {
        while(UART_Get_Data(IMU_UART) != 0x55);
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
                    IMU.AX = ((int16_t)(buffer[1]|(buffer[2] << 8))) / 32768.0f * 16 * 9.8f - Noise.AX;
                    IMU.AY = ((int16_t)(buffer[3]|(buffer[4] << 8))) / 32768.0f * 16 * 9.8f - Noise.AY;
                    break;
                }
                case ANG:
                {
                    IMU.Z = ((int16_t)(buffer[5]|(buffer[6] << 8))) / 32768.0f * 180 - Noise.Z;
                    break;
                }
                case TIME:
                {
                    IMU.t_ms = (u16)(buffer[7]|(buffer[8] << 8));
                    break;
                }
                default:break;
            }
            Check = 0x55;
        }
    }
    return IMU;
}
