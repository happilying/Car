#include "test.h"

void test_begin(void)
{
    Delay_Init();
    Light_Init();
    Location_Init();
    Motor_Init();
    Route_Init();
    S_Y(6.5,1);
    Forward_Y(-6.5);
    Light_Set_Status(ENABLE);
    Delay_Ms(3000);
    Light_Set_Status(DISABLE);
    S_X(-13,1);
    Forward_X(0);
    Light_Set_Status(ENABLE);
    Delay_Ms(3000);
    Light_Set_Status(DISABLE);
    S_Y(-6.5,2);
    Forward_Y(-6.5);
    Light_Set_Status(ENABLE);
    Delay_Ms(3000);
    Light_Set_Status(DISABLE);
    S_X(0,2);
    Forward_X(0);
    Light_Set_Status(ENABLE);
    Delay_Ms(3000);
    Light_Set_Status(DISABLE);
}

void Forward_X(float X)
{
    Locations Location = Location_Get();
    if(X > Location.X)
        Route_to_angle(0);
    else Route_to_angle(-179.9);
    while(Location.X - X > 0.01 || X - Location.X > 0.01)
    {
        Motor_Set_PWM(BASE_PWM);
        if(X < Location.X)
            if(Location.RZ > 0)
                Route_Set_Angle((-Location.RZ + 180) * 1.8);
            else Route_Set_Angle((-Location.RZ - 180) * 1.8);
        else Route_Set_Angle(-Location.RZ * 1.8);
        Location = Location_Get();
    }
    Motor_Set_PWM(0);
}

void Forward_Y(float Y)
{
    Locations Location = Location_Get();
    if(Y > Location.Y)
        Route_to_angle(90);
    else Route_to_angle(-90);
    while(Location.Y - Y > 0.01 || Y - Location.Y > 0.01)
    {
        Motor_Set_PWM(BASE_PWM);
            if(Y > Location.Y)
                Route_Set_Angle( - (Location.RZ - 90) * 1.8);
        else Route_Set_Angle( - (Location.RZ + 90) * 1.8);
        Location = Location_Get();
    }
    Motor_Set_PWM(0);
}

void Route_to_angle(float angle)
{
    Locations Location = Location_Get();
    if(angle - Location.RZ >= 179 || Location.RZ - angle >= 179)
        Location.RZ = -Location.RZ;
    while(Location.RZ - angle >= 1 || angle - Location.RZ >= 1)
    {
        Motor_Set_PWM(BASE_PWM);
        Route_Set_Angle((angle - Location.RZ) * 1.2);
        Location = Location_Get();
        if(angle - Location.RZ >= 180 || Location.RZ - angle >= 180)
            Location.RZ = -Location.RZ;
        char s[50];
        int len = sprintf(s,"%d\r\n",(int)((angle - Location.RZ) * 10));
        UART_Send_Array(UART1, s, len);
    }
    Route_Set_Angle(0);
    Motor_Set_PWM(0);
}

void S_Y(float X,int cont)
{
    Locations Location = Location_Get();
    if(X - Location.X >= 0.5 || Location.X - X >= 0.5)
        if(X < 0)
            Forward_X(X + 0.5);
        else
            Forward_X(X - 0.5);
    if(cont == 1)
        Route_to_angle(-90);
    else Route_to_angle(90);
    while(X - Location.X >= 0.01 || Location.X - X >= 0.01)
    {
        if(X < 0)
            Route_Set_Angle(-(X - Location.X) * 60);
        else Route_Set_Angle((X - Location.X) * 60);
        Motor_Set_PWM(BASE_PWM);
        Location = Location_Get();
    }
    Motor_Set_PWM(0);
}

void S_X(float Y,int cont)
{
    Locations Location = Location_Get();
    if(Y - Location.Y >= 0.5 || Location.Y - Y >= 0.5)
        if(Y < 0)
            Forward_Y(Y + 0.5);
        else
            Forward_Y(Y - 0.5);

    if(cont == 1)
        Route_to_angle(-179.9);
    else Route_to_angle(0);
    while(Y - Location.Y >= 0.01 || Location.Y - Y >= 0.01)
    {
        if(Y < 0)
            Route_Set_Angle(- (Y - Location.Y) * 60);
        else Route_Set_Angle((Y - Location.Y) * 60);
        Motor_Set_PWM(BASE_PWM);
        Location = Location_Get();
    }
    Motor_Set_PWM(0);
}