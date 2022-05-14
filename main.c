#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "globals.h"
#include "io.c"
#include "rpt.c"
#include "serial.h"

volatile bool mustid = false;

bool id_time(struct repeating_timer *t){ // ID timer
    mustid = true;
    return true;
}

void id(rpt *myrpt){ // ID routine
    mustid = false;
    if (!myrpt->tx && myrpt->enabled){
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

void overTone(rpt *myrpt){ // Courtesy tone 
    switch (myrpt->courtesy_select){
        case 0xA:
        switch(myrpt->receiverId){ // RSSI courtesy tones
            case 1: // CTCSS
            if (myrpt->rssi >= RSSI_HIGH) {
                ids("...", myrpt->courtesy_freq);
            } else if (myrpt->rssi <= RSSI_LOW) {
                ids(".", myrpt->courtesy_freq);
            } else {
                ids("..", myrpt->courtesy_freq);
            }
            break;
            case 2: // Internet
            ids("-.-", myrpt->courtesy_freq);
            break;
            case 3: // No CTCSS / TB
            ids("-..", myrpt->courtesy_freq);
            break;
            default:
            ids(".", myrpt->courtesy_freq);
        }
        break;
        case 0x1:
        switch(myrpt->receiverId){ // Ringing courtesy tones
            case 1: // CTCSS
            for (int i = 0; i <= 1000; i++){
                set_pwm_pin(PIP,1100,2000-i*2);
                sleep_ms(1);
                if(rx()){
                    set_pwm_pin(PIP,0,0);
                    break;
                    }
                }
            break;
            case 2: // Internet
            ids("-.-", myrpt->courtesy_freq);
            break;
            case 3: // No CTCSS / TB
            for (int i = 0; i <= 500; i++){
                set_pwm_pin(PIP,900,2000-i*4);
                sleep_ms(2);
                if(rx()){
                    set_pwm_pin(PIP,0,0);
                    break;
                    }
                }            
            break;
            default:
            ids(".", myrpt->courtesy_freq);
        }
        break;
        default:
        break;
    }
    
}

void printDebug(char* message, int data){ // Serial debug output
#ifdef DEBUG
    printf(message, data);
#endif
    return;
}

void ack(rpt *myrpt){ // Send "C" to let user know the command was received.
    sleep_ms(250);
    ids("-.-.", myrpt->cw_freq);
    myrpt->ack_c = 1;
}

int main()
{
#ifdef DEBUG
    stdio_init_all();
#endif
    
    uart_init(uart0, 115200);

    rfMute(1); // Mute audio path
    extMute(1); // Mute aux input
    int cc = 0; // DTMF counter
    uint8_t input[10] = {0}; // DTMF command store
    counter Rpt_c;
    counter *my_c = &Rpt_c;
    rpt Rpt;
    rpt *myrpt = &Rpt;

    myrpt->ack_c = 0;
    myrpt->allow_c = 1;
    myrpt->beacon_id = 1;
    myrpt->courtesy_freq=COURTESY_TONE_FREQ;
    myrpt->courtesy_select = 0x1;
    myrpt->clid = 0;
    myrpt->cw_freq=CW_BEACON_FREQ;
    myrpt->enabled=1;
    myrpt->hangTime=HANGTIME;
    myrpt->idle=1;
    myrpt->latchTime=LATCHTIME;
    myrpt->mode=MODE;
    myrpt->sampleTime=SAMPLETIME;
    myrpt->tail_pips = 0;
    myrpt->timeout_enabled = 1;
    myrpt->timeOut=TIMEOUT;
    myrpt->tt = 0;
    tx(myrpt->tx = 0);
    myrpt->receiver_protected = 0;
    myrpt->rx = 0;
    my_c->latch_c=0;
    my_c->tail_c=TAIL_ID;
    my_c->tail_pips=0;

    struct repeating_timer timer;
    add_repeating_timer_ms(ID, id_time, NULL, &timer); // Setup ID timer

    for (int i = 0; i < sizeof(pins_num)/sizeof(pins_num[0]); i++){ // Init GPIO
        gpio_init(pins_num[i]);
    }
    
    for (int i = 0; i < sizeof(dir_num)/sizeof(dir_num[0]); i++){ // Set GPIO direction
        gpio_set_dir(dir_num[i][0], dir_num[i][1]);
    }

    adc_init(); // Setup ADC for RSSI sampling
    adc_gpio_init(RSSI);
    adc_select_input(0);

    // Send boot message to host
    usend(boot_msg);

    id(myrpt); // Send ID on boot

    while (1){
        if (myrpt->idle || myrpt->receiver_protected){ // Repeater mode selection
            switch(myrpt->mode){
            case 0: // carrier only
            if (myrpt->rx)
                myrpt->idle=0;
            break;
            case 1: // CTCSS or toneburst
            if (myrpt->rx && myrpt->ctcss_decode && !myrpt->tt || myrpt->rx && myrpt->tb && !myrpt->tt && !myrpt->receiver_protected)
                myrpt->idle = 0;
            break;
            case 2: // CTCSS
            if (myrpt->rx && myrpt->ctcss_decode && !myrpt->tt)
                myrpt->idle = 0;
            break;
            case 3: // toneburst
            if (myrpt->rx && myrpt->tb && !myrpt->tt)
                myrpt->idle = 0;
            break;
            case 4: // carrier only and internet gateway
            if (myrpt->rx || myrpt->ext_rx)
                myrpt->idle=0;
            break;
            case 5: // CTCSS or toneburst and internet gateway
            if (myrpt->rx && myrpt->ctcss_decode && !myrpt->tt || myrpt->rx && myrpt->tb && !myrpt->tt && !myrpt->receiver_protected || myrpt->ext_rx)
                myrpt->idle = 0;
            break;
            case 6: // CTCSS and internet gateway
            if (myrpt->rx && myrpt->ctcss_decode && !myrpt->tt || myrpt->ext_rx)
                myrpt->idle = 0;
            break;
            case 7: // toneburst and internet gateway
            if (myrpt->rx && myrpt->tb && !myrpt->tt || myrpt->ext_rx)
                myrpt->idle = 0;
            break;
            default:
            break;
            }
        }

        if (myrpt->rx && !myrpt->idle && !myrpt->tt){ // If a valid signal is present on input
            my_c->hang_c = 0;
            my_c->sample_c = time_us_64();
            my_c->timeOut_c = my_c->sample_c;
            myrpt->tt = 1;
            if (myrpt->enabled)
                tx(myrpt->tx = 1);
            rfMute(0);
        }
        
        if (myrpt->ext_rx && !myrpt->idle && !myrpt->tt){ // If a signal is present on AUX input
            my_c->hang_c = 0;
            my_c->timeOut_c = time_us_64();
            myrpt->tt = 1;
            if (myrpt->enabled)
                tx(myrpt->tx = 1);
            myrpt->latch = 1;
            extMute(0);
        }

        if (myrpt->rx && myrpt->tt && !myrpt->latch && my_c->latch_c == 0) // Start the latch timer
            my_c->latch_c=time_us_64();

        if (myrpt->rx && time_us_64() - my_c->latch_c >= myrpt->latchTime && !myrpt->latch && !myrpt->idle){ // If the user has latched
            myrpt->latch = 1;
            my_c->latch_c = 0;
        }

        if (dtmfDetect() && myrpt->rx){ // DTMF decode
            cc++;
            printDebug("DTMF Detect line\n", 0);
            uint8_t code = getCode();
            rfMute(1);
            ids("--", myrpt->cw_freq);
            switch(code){
                case USER_CONTROL: // User control
                if (!myrpt->allow_c || cc != 1) // Is user control enabled?
                    break;
                for (int i = 0; i < 12; i++){ // 3 seconds is enough
                    sleep_ms(250);
                    switch (getCode()){
                        case USER_CONTROL: // User hasn't entered anything so do nothing
                        break;
                        case REQUEST_ID: // User has requested ID
                        ack(myrpt);
                        sleep_ms(100);
                        id(myrpt);
                        break;
                        case NORMAL_HANG: // User has requested normal hangtime
                        ack(myrpt);
                        myrpt->hangTime=HANGTIME;
                        break;
                        case ALT_HANG: // User has requested short hangtime
                        ack(myrpt);
                        myrpt->hangTime=HANGTIME/4;
                        default:
                        break;
                    }
                    if (myrpt->ack_c){ // Break out if we received a command
                        myrpt->ack_c = 0;
                        break;
                    }
                }
                cc=0;
                break;
                case DTMF_ALLSTAR_START:
                cc=0;
                break;
                case DTMF_RESET_DIGIT:
                cc=0;
                for (int i = 0; i < 9; i++){
                    input[i] = 0;
                }
                break;
                default:
                input[cc-1] = code;
                break;
            }
            if (cc >= 6) {
                int pass = 0;
                for (int i = 0; i < 3; i++){
                    printf("%d\n", input[i]);
                    pass = pass * 10;
                    pass = pass + input[i];
                }
                if (pass == PASSCODE){ // DTMF admin mode
                    for (int j = 0; j < 5; j++){
                        printf("FULL: AP: %d, VAL: %d\n", j, input[j]); // Print command over serial
                    }
                    sleep_ms(3000);
                    ids(".--.", myrpt->cw_freq);
                    switch(input[3]){
                        case ENABLE_USER_COMMANDS: // Disable/Enable user commands
                        myrpt->allow_c=input[4];
                        ids("..-", myrpt->cw_freq);
                        break;
                        case ENABLE_TRANSMITTER: // Disable/Enable repeater transmitter
                        myrpt->enabled=input[4];
                        if (!myrpt->tx)
                            tx(myrpt->tx = 1);
                        sleep_ms(1000);
                        ids("..--..", myrpt->cw_freq);
                        break;
                        case ENABLE_RECEIVER_PROTECTED:
                        myrpt->receiver_protected=input[4]; // Disable/Enable repeater protected receiver
                        ids(".--.", myrpt->cw_freq);
                        break;
                        case ENABLE_REPEATER_TIMEOUT:
                        myrpt->timeout_enabled=input[4]; // Disable/Enable repeater timeout
                        ids("---", myrpt->cw_freq);
                        break;
                        case ENABLE_TAIL_PIPS:
                        myrpt->tail_pips=input[4]; // Disable/Enable tail pips
                        ids("...", myrpt->cw_freq);
                        break;
                        case ENABLE_BEACON_ID:
                        myrpt->beacon_id=input[4]; // Disable/Enable beacon ID
                        ids("-...", myrpt->cw_freq);
                        break;
                        case COURTESY_TONE_SELECT:
                        myrpt->courtesy_select=input[4]; // Courtesy tone select
                        ids("-", myrpt->cw_freq);
                        break;
                        default:
                        break;
                    }
                }
                cc = 0;
            }
            printf("DTMF: %d\n", code);
            sleep_ms(250);
            rfMute(0);
        }

        if(!myrpt->rx && !myrpt->ext_rx && myrpt->tt || !myrpt->ctcss_decode && myrpt->receiver_protected){ // If valid signal has gone
            myrpt->tt = 0;
            if (myrpt->receiver_protected)
                myrpt->idle = 1;
#ifdef MUTE_ON_COS
            rfMute(1);
            extMute(1);
#endif
        }

        if (myrpt->latch && myrpt->rx && time_us_64() - my_c->sample_c >= myrpt->sampleTime){ // Sample RSSI input
            my_c->sample_c = time_us_64();
            myrpt->rssi = adc_read();
        }

        if ((myrpt->rx || myrpt->ext_rx) && time_us_64() - my_c->timeOut_c >= myrpt->timeOut && myrpt->timeout_enabled){ // TIMEOUT
            rfMute(1);
            if(myrpt->tx)
                ids(".-.-.-.-.-.-.-.-.-.-", 400);
            tx(myrpt->tx=0);
        }

        if(!myrpt->tt && myrpt->tx){ // Start the hangtimer
            if (my_c->hang_c == 0)
                my_c->hang_c=time_us_64();

            if (myrpt->latch){
                if (time_us_64() - my_c->hang_c <= 500 && time_us_64() - my_c->timeOut_c >= myrpt->sampleTime){
                    sleep_ms(750);
                    overTone(myrpt);
                } else if (myrpt->tail_pips && time_us_64() - my_c->hang_c > 2000000ULL && time_us_64() - my_c->tail_pips >= myrpt->sampleTime/2){
                    ids(".", myrpt->courtesy_freq-200);
                    my_c->tail_pips = time_us_64();
                }
                if (time_us_64() - my_c->hang_c >= myrpt->hangTime){
                    if (my_c->tail_c >= TAIL_ID-1){
                        my_c->tail_c = 0;
                        myrpt->cw_freq=CW_CLOSEDOWN_FREQ;
                        myrpt->clid = 1;
                        id(myrpt);
                        if(rx())
                            continue;
                        }
                    myrpt->latch = 0;
                }
            } else {
                sleep_ms(250);
                myrpt->idle = 1;
                my_c->latch_c = 0;
                tx(myrpt->tx = 0);
                my_c->tail_c++;
            }
        }

        if(mustid && myrpt->beacon_id) // Check if we must ID
            id(myrpt);
        else if (mustid && myrpt->tx)
            id(myrpt);


        getState(myrpt); // Check input states
    }
    return 0;
}