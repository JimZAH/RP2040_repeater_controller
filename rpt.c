#include "rpt.h"

void getState(rpt *myrpt){
    myrpt->ctcss_decode = ctcssDetect();
    myrpt->rx = rx();
    myrpt->ext_rx = extRx();
    myrpt->tb = toneBurst();

    if (myrpt->rx)
        myrpt->receiverId = 1;
    else if (myrpt->ext_rx)
        myrpt->receiverId = 2;
}