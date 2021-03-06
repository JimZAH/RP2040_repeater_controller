#include "rpt.h"

int getCode(){ // Get DTMF digit
    return gpio_get_all() & DTMF_MASK;
}

void getState(rpt *myrpt){ // Check the repeater input states
    myrpt->ctcss_decode = ctcssDetect();
    myrpt->rx = rx();
    myrpt->ext_rx = extRx();
    myrpt->tb = toneBurst();

    if (myrpt->rx && myrpt->ctcss_decode)
        myrpt->receiverId = 1;
    else if (myrpt->rx)
        myrpt->receiverId = 3;
    else if (myrpt->ext_rx)
        myrpt->receiverId = 2;
}