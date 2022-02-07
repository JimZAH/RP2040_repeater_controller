typedef struct counters{
    uint64_t hang_c;
    uint64_t latch_c;
    uint64_t sample_c;
    uint8_t tail_c;
    uint64_t timeOut_c;
}counter;


typedef struct repeater{
    uint courtesy_freq;
    bool clid;
    bool ctcss_decode;
    uint cw_freq;
    bool ext_rx;
    bool ext_tx;
    uint64_t hangTime;
    bool idle;
    bool latch;
    uint64_t latchTime;
    uint8_t mode;
    uint8_t receiverId;
    bool receiver_protected;
    bool rx;
    uint16_t rssi;
    uint64_t sampleTime;
    bool tt;
    bool tx;
    uint64_t timeOut;
    bool tb;
    bool timedOut;
}rpt;