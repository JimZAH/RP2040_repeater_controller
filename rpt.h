typedef struct counters{
    uint64_t hang_c; // Hangtime
    uint64_t latch_c; // Latch
    uint64_t sample_c; // RSSI sample
    uint8_t tail_c; // Tail message (ID)
    uint64_t timeOut_c; // Timeout
}counter;


typedef struct repeater{
    bool ack_c; // Command ACK flag
    bool allow_c; // User control enable flag
    uint courtesy_freq;
    bool clid; // Close down ID flag
    bool ctcss_decode;
    uint cw_freq;
    bool enabled; // Repeater enabled
    bool ext_rx; // Internet gateway
    bool ext_tx; // Internet gateway
    uint64_t hangTime;
    bool idle; // Idle flag
    bool latch;
    uint64_t latchTime;
    uint8_t mode; // Repeater operation mode
    uint8_t receiverId; // Receiver ID flag
    bool receiver_protected;
    bool rx;
    uint16_t rssi;
    uint64_t sampleTime;
    bool tt;
    bool tx;
    uint64_t timeOut;
    bool tb; // Tone burst
    bool timeout_enabled;
    bool timedOut;
}rpt;