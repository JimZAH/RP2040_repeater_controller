#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "globals.h"
#include "io.c"
#include "rpt.h"

volatile bool mustid = false;

bool id_time(struct repeating_timer *t){
    mustid = true;
    return true;
}

void id(rpt *myrpt){
    mustid = false;
    if (!myrpt->tx){
        myrpt->tx = 1;
        tx(myrpt->tx);
        sleep_ms(500);
    }
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

char* reportRSSI(rpt *myrpt){
    if (myrpt->rssi >= RSSI_HIGH) {
        return '....';
    if (myrpt->rssi <= RSSI_LOW) {
        return '..';
        }
    }
    return '...';
}

int main()
{
    rfMute(1);
    extMute(1);
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

   // for (int i = 0; i < sizeof(pins_num); i++){
   //     gpio_init(pins_num[i]);
   // }

   // for (int i = 0; i < sizeof(dir_num); i++){
   //     gpio_set_dir(dir_num[i][0], dir_num[i][1]);
   // }

    gpio_init(COS);
    gpio_init(CTCSS);
    gpio_init(EXT_RX);
    gpio_init(EXT_PTT);
    gpio_init(RF_MUTE);
    gpio_init(EXT_MUTE);
    gpio_init(PTT);
    gpio_init(PIP);
    gpio_init(PTT_LED);
    gpio_init(RSSI);
    gpio_set_dir(COS, GPIO_IN);
    gpio_set_dir(CTCSS, GPIO_IN);
    gpio_set_dir(EXT_RX, GPIO_IN);
    gpio_set_dir(EXT_PTT, GPIO_OUT);
    gpio_set_dir(RF_MUTE, GPIO_OUT);
    gpio_set_dir(EXT_MUTE, GPIO_OUT);
    gpio_set_dir(PTT, GPIO_OUT);
    gpio_set_dir(PIP, GPIO_OUT);
    gpio_set_dir(PTT_LED, GPIO_OUT);
    gpio_set_dir(RSSI, GPIO_IN);


    adc_init();
    adc_gpio_init(RSSI);
    adc_select_input(0);


    while (1){
        if (rx() && !myrpt->rx){ // If a valid signal is present on input
            my_c->hang_c = 0;
            my_c->sample_c = time_us_64();
            myrpt->rx = 1;
            myrpt->tx = 1;
            tx(myrpt->tx);
            rfMute(0);
        }

        if (myrpt->rx && !myrpt->latch && my_c->latch_c == 0) // Start the latch timer
            my_c->latch_c=time_us_64();

        if (myrpt->rx && time_us_64() - my_c->latch_c >= myrpt->latchTime && !myrpt->latch){ // If the user has latched
            myrpt->latch = 1;
            my_c->latch_c = 0;
        }

        if(!rx() && myrpt->rx){ // If valid signal has gone
            myrpt->rx = false;
            rfMute(1);
        }

        if (myrpt->latch && myrpt->rx && time_us_64() - my_c->sample_c >= myrpt->sampleTime){
            my_c->sample_c = time_us_64();
            myrpt->rssi = adc_read();
        }

        if(!myrpt->rx && myrpt->tx){ // Start the hangtimer
            if (my_c->hang_c == 0)
                my_c->hang_c=time_us_64();

            if (myrpt->latch){
                if (time_us_64() - my_c->hang_c <= 500){
                    sleep_ms(750);
                    ids(reportRSSI(myrpt), 1240);
                }
                if (time_us_64() - my_c->hang_c >= myrpt->hangTime){

#ifdef CLOSE_DOWN_ID
                    id(myrpt);
                    if(rx())
                        continue;
#endif
                    myrpt->tx = 0;
                    myrpt->latch = 0;
                    tx(myrpt->tx);
                }
            } else {
                myrpt->tx = 0;
                my_c->latch_c = 0;
                tx(myrpt->tx);
            }

        }
#ifdef BEACON_ID
        if(mustid) // Check if we must ID
            id(myrpt);
#else
        if(mustid && myrpt->tx) // Check if we must ID and in transmit mode
            id(myrpt);
#endif
    }
    return 0;
}