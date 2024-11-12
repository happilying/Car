#include "IMU.h"
#include "Command_IMU.h"
#include "Delay.h"

/**
 * @fn      IMU_Init
 *
 * @brief   初始化IMU模块，并校准Z旋转与加速度
 * 
 * @return  IMU噪声平均值与完成初始化时间，
 */
IMU_State IMU_Init(void)
{
    IMU_State Noise = {0};
    u8 Noises = Noise_Count;

    Delay_Init();
    UART_Init(IMU_UART,9600);
    Delay_Ms(1000);
    UART_Clear_Buffer(IMU_UART);
    UART_Send_Array(IMU_UART,Unlock,sizeof(Unlock));
    Delay_Ms(200);
    UART_Send_Array(IMU_UART,baudrat,sizeof(baudrat));
    UART_Set_baudrate(IMU_UART, IMU_Baudrate);
    UART_Send_Array(IMU_UART,Unlock,sizeof(Unlock));
    Delay_Ms(200);
    UART_Send_Array(IMU_UART,Z_Calibration,sizeof(Z_Calibration));
    Delay_Ms(3000);
    UART_Send_Array(IMU_UART,ACC_Calibration,sizeof(ACC_Calibration));
    Delay_Ms(5000);
    UART_Send_Array(IMU_UART,Save,sizeof(Save));
    for(u8 i = 0;i <= Noise_Count - 1;i++)
    {
        IMU_State IMU = IMU_Get_Data();
        if(IMU.t_ms == 0)
        {
            Noises--;
            continue;
        }
        Noise.AX += IMU.AX;
        Noise.AY += IMU.AY;
        Noise.Z += IMU.Z;
        Noise.t_ms = IMU.t_ms;
    }
    Noise.AX /= Noises;
    Noise.AY /= Noises;
    Noise.Z /= Noises;
    return Noise;
}

IMU_State IMU_Get_Data(void)
{
    IMU_State IMU = {0};
    u8 buffer[9] = {0};
    u8 Check = 0x55;
    if(!UART_Get_Length(IMU_UART))
    {
        return IMU;
    }
    reget:for(u8 j = 0;j <= 2;j++)
    {
        while(UART_Get_Data(IMU_UART) != 0x55);
        Check = 0x55;
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
                    IMU.AX = ((int16_t)(buffer[1]|(buffer[2] << 8))) / 32768.0f * 16 * 9.8f;
                    IMU.AY = ((int16_t)(buffer[3]|(buffer[4] << 8))) / 32768.0f * 16 * 9.8f;
                    break;
                }
                case ANG:
                {
                    IMU.Z = ((int16_t)(buffer[5]|(buffer[6] << 8))) / 32768.0f * 180;
                    break;
                }
                case TIME:
                {
                    if(buffer[1] != 0 | buffer[2] != 0 | buffer[3] != 0)
                    {
                        goto reget;
                    }
                    IMU.t_ms = (u16)(buffer[7]|(buffer[8] << 8));
                    j = 0;
                    break;
                }
                default:break;
            }
        }
        else goto reget;
    }
    return IMU;
}
