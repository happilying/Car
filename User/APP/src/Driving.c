#include "Location.h"
#include "Motor.h"
#include "Ranging.h"
#include "Route.h"
#include "stdint.h"
#include "Delay.h"
#include "math.h"

#define OBSTACLE_DISTANCE 20
#define DOOR_WIDTH 50
#define CAR_SPEED 10
#define BASE_PWM 50
#define DOOR_ALIGN_THRESHOLD 5
#define CIRCLE_RADIUS 100 // 环形道路的近似半径（以厘米为单位）

// 函数原型
void stop_car(void);
void move_forward(void);
void drive_s(uint8_t driving_s);
void drive_3s(void);
void drive_500ms(void);
void align_at_door(int areObstaclesOnBothSides);
void navigation_to_next_checkpoint(int areObstaclesOnBothSides);

// 全局变量
Distances distances;
float current_angle = 0.0; // 跟踪汽车当前的方向角度

int main()
{
    // 初始化组件
    Location_Init();
    Ranging_Init();
    Motor_Init();
    Route_Init();
    Delay_Init();

    // 设置初始速度
    Motor_Set_PWM(BASE_PWM);

    // 导航顺序
    navigation_to_next_checkpoint(0); // 导航到 B 点（两侧都有障碍物）
    drive_3s();                       // 通过门

    navigation_to_next_checkpoint(1); // 导航到 C 点（仅限右侧障碍物）
    drive_3s();                       // 通过门

    navigation_to_next_checkpoint(0); // 导航到 D 点（两边都有障碍物）
    drive_3s();                     // 通过门

    navigation_to_next_checkpoint(0); // 返回 A 点
    drive_3s();                     // 通过门

    stop_car(); // 完成循环后停止汽车
    return 0;
}

// 停止汽车
void stop_car(void)
{
    Motor_Set_PWM(0);
}

// 向前移动汽车
void move_forward(void)
{
    Motor_Set_PWM(BASE_PWM);
}

// 行驶特定秒数
void drive_s(uint8_t driving_s)
{
    move_forward();
    Delay_Ms(driving_s * 1000);
    stop_car();
}

// 行驶 3 秒
void drive_3s(void)
{
    drive_s(3);
}

// 行驶 500 毫秒
void drive_500ms(void)
{
    move_forward();
    Delay_Ms(500);
    stop_car();
}

// 调整汽车在环形道路上的角度
void adjust_angle_for_circular_path(void)
{
    // 计算环形道路的角度增量变化
    float delta_angle = CAR_SPEED / (float)CIRCLE_RADIUS;

    // 更新当前角度并为汽车设置
    current_angle += delta_angle;
    if (current_angle > 360.0) // 将角度保持在 0-360 度之间
        current_angle -= 360.0;

    Route_Set_Angle(current_angle - 90.0); // 减去 90 度以与转弯轴对齐
}

// 将汽车对准门口
void align_at_door(int areObstaclesOnBothSides)
{
    stop_car();
    while (1)
    {
        distances = Ranging_Get_Distance();

        if (areObstaclesOnBothSides)
        {
            // 两侧都有障碍物（全门）
            if (distances.Left <= OBSTACLE_DISTANCE || distances.right <= OBSTACLE_DISTANCE)
            {
                if (distances.Left > distances.right)
                {
                    Route_Set_Angle(current_angle - 5); // 左转
                }
                else
                {
                    Route_Set_Angle(current_angle + 5); // 右转
                }
                drive_500ms();
            }
            else
            {
                break; // 对齐
            }
        }
        else
        {
            // 仅右侧障碍物（部分门）
            if (distances.right <= OBSTACLE_DISTANCE)
            {
                Route_Set_Angle(current_angle - 5); // 左转
                drive_500ms();
            }
            else
            {
                break; // 对齐
            }
        }
    }
}

// 导航到下一个检查点
void navigation_to_next_checkpoint(int areObstaclesOnBothSides)
{
    move_forward();
    while (1)
    {
        distances = Ranging_Get_Distance();

        // 当侧面未检测到障碍物时停止
        if (!(distances.Left <= OBSTACLE_DISTANCE || distances.right <= OBSTACLE_DISTANCE))
        {
            break;
        }

        // 持续调整环形道路的角度
        adjust_angle_for_circular_path();
    }
    stop_car();
    align_at_door(areObstaclesOnBothSides);
}
