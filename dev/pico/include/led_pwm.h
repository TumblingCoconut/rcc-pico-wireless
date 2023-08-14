#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <math.h>
#include <stdlib.h>
#include "rcc_pins.h"
using namespace std;

typedef struct
{
    Servo blue;
    Servo red;
    Servo green;

} RGBLED;

void LEDinit(RGBLED *m, uint blue, uint red, uint green, uint freq)
{
    MotorInitGPIO(blue);
    MotorInitGPIO(red);
    MotorInitGPIO(green);
    gpio_set_function(blue, GPIO_FUNC_PWM);
    gpio_set_function(red, GPIO_FUNC_PWM);
    gpio_set_function(green, GPIO_FUNC_PWM);
    m->blue.slice = pwm_gpio_to_slice_num(blue);
    m->red.slice = pwm_gpio_to_slice_num(red);
    m->green.slice = pwm_gpio_to_slice_num(green);

    m->blue.chan = pwm_gpio_to_channel(blue);
    m->red.chan = pwm_gpio_to_channel(red);
    m->green.chan = pwm_gpio_to_channel(green);

    m->blue.gpio = blue;
    m->red.gpio = red;
    m->green.gpio = green;

    m->blue.resolution = pwm_set_freq_duty(m->blue.slice, m->blue.chan, freq, 0);
    m->red.resolution = pwm_set_freq_duty(m->red.slice, m->red.chan, freq, 0);
    m->green.resolution = pwm_set_freq_duty(m->green.slice, m->green.chan, freq, 0);
}

// Turn all leds on
void LEDOn(RGBLED *m)
{
    pwm_set_enabled(m->blue.slice, true);
    pwm_set_enabled(m->red.slice, true);
    pwm_set_enabled(m->green.slice, true);
    m->blue.on = true;
    m->red.on = true;
    m->green.on = true;
}

void LEDPower(RGBLED *m, int blue, int red, int green)
{
    // pwm_set_dutyF(m->left.slice, m->left.chan, std::abs(lp));
    // pwm_set_dutyF(m->right.slice, m->right.chan, std::abs(rp));
    pwm_set_duty(m->blue.gpio, abs(blue));
    pwm_set_duty(m->red.gpio, abs(red));
    pwm_set_duty(m->green.gpio, abs(green));
}