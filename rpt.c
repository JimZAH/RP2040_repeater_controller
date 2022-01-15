#include "rpt.h"

void getState(rpt *myrpt){
    myrpt->rx = rx();
    myrpt->ext_rx = extRx();

    if (myrpt->rx)
        myrpt->receiverId = 1;
    else if (myrpt->ext_rx)
        myrpt->receiverId = 2;
}