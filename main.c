#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "globals.h"
#include "io.c"
#include "rpt.c"

volatile bool mustid = false;

bool id_time(struct repeating_timer *t){
    mustid = true;
    return true;
}

void id(rpt *myrpt){
    mustid = false;
    if (!myrpt->tx){
        tx(myrpt->tx = 1);
        sleep_ms(500);
    }
    if (myrpt->clid){
        ids("-..", myrpt->cw_freq);
        ids(".", myrpt->cw_freq);
        sleep_ms(space*2);
    }
    for (int i=0; i < sizeof(morse) / sizeof(morse[0]); i++){
    if (i > 4){
      sleep_ms(space*2);
        }
     ids(morse[i], myrpt->cw_freq);
    }
    myrpt->cw_freq=CW_BEACON_FREQ;
    myrpt->clid = 0;
}

void ids(char* s, int tone){
  int j = 0;
  while (s[j] != NULL){
      rfMute(!rx()); // Unmute the audio path if we are receiving.
      idm(s[j], tone);
      j++;
  }
  sleep_ms(dit*2);
}

void idm(char c, int tone){
  set_pwm_pin(PIP,tone,2500);
  if (c == '.'){
    sleep_ms(dit);
  } else {
    sleep_ms(dah);
  }
  set_pwm_pin(PIP,0,0);
  sleep_ms(dit);
}

char* overTone(rpt *myrpt){

    switch(myrpt->receiverId){
        case 1:
            if (myrpt->rssi >= RSSI_HIGH) {
              return "....";
            if (myrpt->rssi <= RSSI_LOW) {
              return "..";
              }
            }
            return "...";
        case 2:
            return "-.-";
        default:
            return ".";
    }
    
}

void printDebug(char* message, int data){
#ifdef DEBUG
    printf(message, data);
#endif
    return;
}

int main()
{
#ifdef DEBUG
    stdio_init_all();
#endif
    rfMute(1);
    extMute(1);
    int cc = 0; // DTMF counter
    counter Rpt_c;
    counter *my_c = &Rpt_c;
    rpt Rpt;
    rpt *myrpt = &Rpt;

    myrpt->courtesy_freq=COURTESY_TONE_FREQ;
    myrpt->clid = 0;
    myrpt->cw_freq=CW_BEACON_FREQ;
    myrpt->hangTime=HANGTIME;
    myrpt->idle=1;
    myrpt->latchTime=LATCHTIME;
    myrpt->mode=MODE;
    myrpt->sampleTime=SAMPLETIME;
    myrpt->timeOut=TIMEOUT;
    myrpt->tt = 0;
    tx(myrpt->tx = 0);
    myrpt->receiver_protected = 0;
    myrpt->rx = 0;
    my_c->latch_c=0;

    struct repeating_timer timer;
    add_repeating_timer_ms(ID, id_time, NULL, &timer); // Setup ID timer

    for (int i = 0; i <= sizeof(pins_num)/sizeof(pins_num[0]); i++){
        gpio_init(pins_num[i]);
    }
    
    for (int i = 0; i <= sizeof(dir_num)/sizeof(dir_num[0]); i++){
        gpio_set_dir(dir_num[i][0], dir_num[i][1]);
    }

    adc_init();
    adc_gpio_init(RSSI);
    adc_select_input(0);

    id(myrpt);

    while (1){

        if (myrpt->idle || myrpt->receiver_protected){
            switch(myrpt->mode){
            case 0: // carrier only
            if (myrpt->rx)
                myrpt->idle=0;
                printDebug("%s", "Carrier");
            break;
            case 1: // CTCSS or toneburst
            if (myrpt->rx && myrpt->ctcss_decode && !myrpt->tt || myrpt->rx && myrpt->tb && !myrpt->tt)
                myrpt->idle = 0;
                printDebug("%s", "CTCSS/Toneburst");
            break;
            case 2: // CTCSS
            if (myrpt->rx && myrpt->ctcss_decode && !myrpt->tt)
                myrpt->idle = 0;
                printDebug("%s", "CTCSS");
            break;
            case 3: // toneburst
            if (myrpt->rx && myrpt->tb && !myrpt->tt)
                myrpt->idle = 0;
            default:
            break;
            }
        }

        if (myrpt->rx && !myrpt->idle && !myrpt->tt){ // If a valid signal is present on input
            my_c->hang_c = 0;
            my_c->sample_c = time_us_64();
            myrpt->tt = 1;
            tx(myrpt->tx = 1);
            rfMute(0);
        }

        if (myrpt->rx && myrpt->tt && !myrpt->latch && my_c->latch_c == 0) // Start the latch timer
            my_c->latch_c=time_us_64();

        if (myrpt->rx && time_us_64() - my_c->latch_c >= myrpt->latchTime && !myrpt->latch && !myrpt->idle){ // If the user has latched
            myrpt->latch = 1;
            my_c->latch_c = 0;
        }

        if (dtmfDetect() && myrpt->latch){
            cc++;
            printDebug("DTMF Detect line\n", NULL);
            uint8_t a = gpio_get_all();
            a = a & DTMF_MASK;
            printDebug("DTMF: %d\n", a);
            sleep_ms(1000);
        }

        if(!myrpt->rx && myrpt->tt || !myrpt->ctcss_decode && myrpt->receiver_protected){ // If valid signal has gone
            myrpt->tt = 0;
            if (myrpt->receiver_protected)
                myrpt->idle = 1;
            rfMute(1);
        }

        if (myrpt->latch && myrpt->rx && time_us_64() - my_c->sample_c >= myrpt->sampleTime){
            my_c->sample_c = time_us_64();
            myrpt->rssi = adc_read();
        }

        if(!myrpt->tt && myrpt->tx){ // Start the hangtimer
            if (my_c->hang_c == 0)
                my_c->hang_c=time_us_64();

            if (myrpt->latch){
                if (time_us_64() - my_c->hang_c <= 500){
                    sleep_ms(1000);
                    ids(overTone(myrpt), myrpt->courtesy_freq);
                }
                if (time_us_64() - my_c->hang_c >= myrpt->hangTime){
#ifdef CLOSE_DOWN_ID
                    myrpt->cw_freq=CW_CLOSEDOWN_FREQ;
                    myrpt->clid = 1;
                    id(myrpt);
                    if(rx())
                        continue;
#endif
                    myrpt->latch = 0;
                }
            } else {
                sleep_ms(250);
                myrpt->idle = 1;
                my_c->latch_c = 0;
                tx(myrpt->tx = 0);
            }
        }
#ifdef BEACON_ID
        if(mustid) // Check if we must ID
            id(myrpt);
#else
        if(mustid && myrpt->tx) // Check if we must ID and in transmit mode
            id(myrpt);
#endif
        getState(myrpt);
    }
    return 0;
}