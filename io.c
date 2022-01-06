#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "io.h"



void extMute(bool state)
{
#ifdef EXT_MUTE_INVERT
    if (state)
        gpio_put(EXT_MUTE, true);
        else
        gpio_put(EXT_MUTE, false);
#else
    if (state)
        gpio_put(EXT_MUTE, false);
        else
        gpio_put(EXT_MUTE, true);
#endif
}

void extPtt(bool state)
{
#ifdef EXT_PTT_INVERT
    if (state)
        gpio_put(EXT_PTT, true);
        else
        gpio_put(EXT_PTT, false);
#else
    if (state)
        gpio_put(EXT_PTT, false);
        else
        gpio_put(EXT_PTT, true);
#endif
}

void tx(bool state)
{
#ifdef PTT_INVERT
    if (state){
        gpio_put(PTT, true);
        } else {
        gpio_put(PTT, false);
        }
#else
    if (state){
        gpio_put(PTT, false);
       } else {
        gpio_put(PTT, true);
      }
#endif
}

void rfMute(bool state)
{
#ifdef RF_MUTE_INVERT
    if (state)
        gpio_put(RF_MUTE, true);
        else
        gpio_put(RF_MUTE, false);
#else
    if (state)
        gpio_put(RF_MUTE, false);
        else
        gpio_put(RF_MUTE, true);
#endif
}

bool rx()
{
#ifdef COS_INVERT
    if (!gpio_get(COS))
        return true;
#else
    if (gpio_get(COS))
        return true;
#endif
    return false;
}

void set_pwm_pin(uint pin, uint freq, uint duty_c) { // duty_c between 0..10000
		gpio_set_function(pin, GPIO_FUNC_PWM);
		uint slice_num = pwm_gpio_to_slice_num(pin);
        pwm_config config = pwm_get_default_config();
		float div = (float)clock_get_hz(clk_sys) / (freq * 10000);
		pwm_config_set_clkdiv(&config, div);
		pwm_config_set_wrap(&config, 10000);
		pwm_init(slice_num, &config, true); // start the pwm running according to the config
		pwm_set_gpio_level(pin, duty_c); //connect the pin to the pwm engine and set the on/off level. 
};