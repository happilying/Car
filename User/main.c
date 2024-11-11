#include "Motor.h"
#include "Route.h"
#include "USART.h"
//#include "Raidar.h"
//#include "Ranging.h"

//u32 count = 0;
int main(void)
{
    int i = 0;
    u8 flag = 0;
    u8 Buffer[3] = {0};
    Motor_Init();
    Route_Init();
    UART_Init(115200, UART1);
    UART_Send_Data(UART1, 0x66);
    UART_Clear_Buffer(UART1);
    while(1)
    {
        while(UART_Get_Length(UART1))
        {
            Buffer[i] = UART_Get_Data(UART1);
            i++;
            flag = 1;
        }
        if(flag)
        {
            UART_Send_Array(UART1,Buffer,sizeof(Buffer));
            i = 0;
            flag = 0;
        }
    }
}
