#include "Motor.h"
#include "Route.h"
//#include "Ranging.h"
#include "GPS.h"

int main(void)
{
    volatile GPS_Location GPS = {0};
    GPS_Init();
    while(1)
    {
        GPS = GPS_Get_Location();
        Delay_Ms(1000);
    }
}
