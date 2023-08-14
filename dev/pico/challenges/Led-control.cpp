#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); // turns on led
    rcc_init_i2c();

    RGBLED leds;
    LEDinit(&leds, RCC_LED_BLUE, RCC_LED_RED, RCC_LED_GREEN, 1000);
    LEDOn(&leds);

    while (true)
    {
        LEDPower(&leds, 100, 100, 100);
    }
}