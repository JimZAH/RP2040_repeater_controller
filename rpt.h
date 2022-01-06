typedef struct counters{
    uint64_t hang_c;
    uint64_t latch_c;
    uint64_t sample_c;
    uint64_t timeOut_c;
}counter;


typedef struct repeater{
    bool ext_rx;
    bool ext_tx;
    uint64_t hangTime;
    bool latch;
    uint64_t latchTime;
    bool rx;
    uint16_t rssi;
    uint64_t sampleTime;
    bool tx;
    uint64_t timeOut;
    bool timedOut;
}rpt;