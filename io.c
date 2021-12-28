#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "io.h"
#include "globals.h"

void tx(bool state)
{
#ifdef PTT_INVERT
    if (state){
        gpio_put(17, true);
        } else {
        gpio_put(17, false);
        }
#else
    if (state){
        gpio_put(17, false);
       } else {
        gpio_put(17, true);
      }
#endif
}

bool rx()
{
#ifdef COS_INVERT
    if (!gpio_get(COS))
        return true;
#else
    if (!gpio_get(COS))
        return true;
#endif
    return false;
}

void set_pwm_pin(uint pin, uint freq, uint duty_c) { // duty_c between 0..10000
		gpio_set_function(pin, GPIO_FUNC_PWM);
		uint slice_num = pwm_gpio_to_slice_num(pin);
        pwm_config config = pwm_get_default_config();
		float div = (float)133000000L / (freq * 10000);
		pwm_config_set_clkdiv(&config, div);
		pwm_config_set_wrap(&config, 10000);
		pwm_init(slice_num, &config, true); // start the pwm running according to the config
		pwm_set_gpio_level(pin, duty_c); //connect the pin to the pwm engine and set the on/off level. 
};