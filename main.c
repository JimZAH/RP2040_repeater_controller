#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "globals.h"
#include "io.c"
#include "io.h"
#include "rpt.h"

volatile bool mustid = false;

bool id_time(struct repeating_timer *t){
    mustid = true;
    return true;
}

void id(){
    mustid = false;
    for (int i=0; i < sizeof(morse) / sizeof(morse[0]); i++){
    if (i > 4){
      sleep_ms(space*2);
        }
     ids(morse[i], 2100);
    }
}

void ids(char* s, int tone){
  int j = 0;
  while (s[j] != NULL){
    idm(s[j], tone);
    j++;
  }
  sleep_ms(dit*2);
}

void idm(char c, int tone){
  set_pwm_pin(16,tone,750);
  if (c == '.'){
    sleep_ms(dit);
  } else {
    sleep_ms(dah);
  }
  set_pwm_pin(16,0,0);
  sleep_ms(dit);
}

int main()
{
    counter Rpt_c;
    counter *my_c = &Rpt_c;
    rpt Rpt;
    rpt *myrpt = &Rpt;

    myrpt->hangTime=HANGTIME;
    myrpt->latchTime=LATCHTIME;
    myrpt->sampleTime=SAMPLETIME;
    myrpt->timeOut=TIMEOUT;
    my_c->latch_c=0;

    struct repeating_timer timer;
    add_repeating_timer_ms(ID, id_time, NULL, &timer); // Setup ID timer

    for (int i = 0; i < sizeof(pins)/2; i++){
        gpio_init(pins[i]);
    }

    for (int i = 0; i < sizeof(dir)/2; i++){
        gpio_set_dir(dir[i][0], dir[i][1]);
    }

    adc_init();
    adc_gpio_init(RSSI);
    adc_select_input(0);


    while (1){
        if (rx() && !myrpt->rx){ // If a valid signal is present on input
            my_c->hang_c = 0;
            my_c->sample_c = time_us_64();
            myrpt->rx = 1;
            myrpt->tx = 1;
            tx(1);
        }

        if (myrpt->rx && !myrpt->latch && my_c->latch_c == 0) // Start the latch timer
            my_c->latch_c=time_us_64();

        if (myrpt->rx && time_us_64() - my_c->latch_c >= myrpt->latchTime && !myrpt->latch){ // If the user has latched
            myrpt->latch = 1;
            my_c->latch_c = 0;
            if(mustid) // Check if we must ID
                id();
        }

        if(!rx() && myrpt->rx){ // If valid signal has gone
            myrpt->rx = false;
        }

        if (myrpt->latch && myrpt->rx && time_us_64() - my_c->sample_c >= myrpt->sampleTime){
            my_c->sample_c = time_us_64();
            myrpt->rssi = adc_read();
            if (mustid)
                id();
        }

        if(!myrpt->rx && myrpt->tx){ // Start the hangtimer
            if (my_c->hang_c == 0)
                my_c->hang_c=time_us_64();

            if (myrpt->latch){
                if (time_us_64() - my_c->hang_c <= 500){
                    sleep_ms(750);
                    ids("...", 300 + myrpt->rssi*4);
                }
                if (time_us_64() - my_c->hang_c >= myrpt->hangTime){
                    id();
                    if(rx())
                        continue;
                    myrpt->tx = 0;
                    myrpt->latch = 0;
                    tx(0);
                }
            } else {
                myrpt->tx = 0;
                my_c->latch_c = 0;
                tx(0);
            }

        }

    }
    return 0;
}