#include "Motor.h"
#include "Route.h"
#include "Ranging.h"


int main(void)
{
    volatile Distances distance = {0};
    Ranging_Init();
    while(1)
    {
        distance = Ranging_Get_Distance();
        Delay_Ms(1000);
    }
}
