#include "rcc_stdlib.h"
using namespace std;
typedef enum
{
    INITIAL,
    WAIT,
    INCREMENT,
    RESET,
    ROTATE
} state_t;

int main()
{
    // Initializations
    stdio_init_all();
    cyw43_arch_init();
    rcc_init_pushbutton();
    rcc_init_potentiometer();
    rcc_init_i2c();
    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    // Initialize motors
    Motor motors;
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000);
    MotorsOn(&motors);
    int motor_speed = 80;

    // Initialize lidar
    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    state_t state = INITIAL;
    uint32_t prev_time = 0;
    uint32_t prev_time2 = 0;
    uint32_t cur_time = 0;
    int servo_pos = 0;
    uint32_t dt = 100000;
    uint32_t dt2 = 1000000;
    uint16_t dist;

    ServoPosition(&s3, 50);
    while (true)
    {
        // Do tons of unnecessary math here

        // Update current time
        cur_time = time_us_32();
        // State0
        switch (state)
        {
        case INITIAL:
            if (!gpio_get(RCC_PUSHBUTTON))
            {
                state = WAIT;
            }
            break;
        case WAIT:
            // Do nothing
            // Check conditions
            if (cur_time - prev_time >= dt)
            {
                state = INCREMENT;
            }
            break;
        case INCREMENT:
            // Increment position and set servo position
            servo_pos = servo_pos + 10;
            ServoPosition(&s3, servo_pos);
            dist = getFastReading(&lidar);
            cout << dist << "\n";

            if (dist < 300){
                state = ROTATE;
            }
            else {
                // Check conditions
                if (servo_pos < 100)
                { // INCREMENT - WAIT
                    prev_time = cur_time;
                    state = WAIT;
                }
                if (servo_pos >= 100)
                { // INCREMENT - RESET
                    prev_time2 = cur_time;
                    state = RESET;
                }
            }
            break;

        case RESET:
            // set servo position to 0 to reset it
            servo_pos = 0;
            ServoPosition(&s3, servo_pos);
            // check conditions
            if (cur_time - prev_time2 >= dt2)
            { // REST - WAIT
                prev_time = cur_time;
                state = WAIT;
            }
            break;
        case ROTATE:
            MotorPower(&motors, 80, -(.983 * 80)); // %motor speed
            break;
        }
    }
}