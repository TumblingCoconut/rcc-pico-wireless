#include "rcc_stdlib.h"
using namespace std;

typedef enum{
    INITIAL,
    SEARCH,
    TRACK, 
    FWD,
    STOP,
    REV
} state_t;

int main(){
    stdio_init_all(); 
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on led
    rcc_init_i2c();

    //init robot's sensors and actuators here~~
    // Initialize motors
    Motor motors;
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000);
    MotorsOn(&motors);

    // Initialize lidar
    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    // Initialize Servo motor
    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    rcc_init_pushbutton(); //set up button

    //rename state_name to something more informative
    state_t state = INITIAL;

    while(true){
        ServoPosition(&s3, 50); // face servo forward
        uint16_t dist = getFastReading(&lidar); // get lidar reading
        switch(state){
            case INITIAL:
                if (!gpio_get(RCC_PUSHBUTTON)) {
                    state = SEARCH;
                }
                break;
            case SEARCH:
                MotorPower(&motors, 70, -(.983 * 70));  // %motor speed 
                if (dist <= 500) {
                    state = TRACK;
                }
                break;
            case TRACK: 
                if (dist > 400){
                    state = FWD;
                }
                if (dist < 300){
                    state = REV;
                }
                if (dist > 1500){
                    state = SEARCH;
                }
                break;
            case FWD:
                MotorPower(&motors, 70, 70);  // %motor speed 
                state = TRACK;
                break;
            case REV:
                MotorPower(&motors, -70, -70);  // %motor speed 
                state = TRACK;
                break;
        }
    }
}