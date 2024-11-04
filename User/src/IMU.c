#include "IMU.h"


void IMU_Init(void)
{
    UART_Init(115200, UART2);
}
