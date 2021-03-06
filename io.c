#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "io.h"


bool ctcssDetect()
{
#ifdef CTCSS_DECODE_INVERT
    if (!gpio_get(CTCSS_DETECT)){
        gpio_put(LAMP_CTCSS_DECODE, 1);
        return true;
    }
#else
    if (gpio_get(CTCSS_DETECT)){
        gpio_put(LAMP_CTCSS_DECODE, 1);
        return true;
    }
#endif
    gpio_put(LAMP_CTCSS_DECODE, 0);
    return false;
}

bool dtmfDetect()
{
#ifdef DTMF_INVERT
    if (!gpio_get(DD))
        return true;
#else
    if (gpio_get(DD))
        return true;
#endif
    return false;
}

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

bool extRx()
{
#ifdef EXT_RX_INVERT
    if (!gpio_get(EXT_RX))
        return true;
#else
    if (gpio_get(EXT_RX))
        return true;
#endif
    return false;
}

void tx(bool state)
{
    if (state)
        gpio_put(LAMP_TX, 1);
    else
        gpio_put(LAMP_TX, 0);
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
    if (!gpio_get(COS)){
        gpio_put(LAMP_SQL, 1);
        return true;
    }
#else
    if (gpio_get(COS)){
        gpio_put(LAMP_SQL, 1);
        return true;
    }
#endif
    gpio_put(LAMP_SQL, 0);
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
        gpio_put(LAMP_CWID, duty_c); // Flash CWID lamp
}

bool toneBurst()
{
#ifdef TB_INVERT
    if (!gpio_get(TONE_BURST)){
        gpio_put(LAMP_TB_DECODE, 1);
        return true;
    }
#else
    if (gpio_get(TONE_BURST)){
        gpio_put(LAMP_TB_DECODE, 1);
        return true;
    }
#endif
    gpio_put(LAMP_TB_DECODE, 0);
    return false;
}