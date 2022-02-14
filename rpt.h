typedef enum dtmf_uc{
    REQUEST_ID = 1,
    NORMAL_HANG,
    ALT_HANG,
    USER_CONTROL = 10
};

typedef enum dtmf_kc{
    ENABLE_USER_COMMANDS = 1,
    ENABLE_TRANSMITTER,
    ENABLE_RECEIVER_PROTECTED,
    ENABLE_REPEATER_TIMEOUT,
    ENABLE_TAIL_PIPS,
    ENABLE_BEACON_ID,
    COURTESY_TONE_SELECT
};


typedef struct counters{
    uint64_t hang_c; // Hangtime
    uint64_t latch_c; // Latch
    uint64_t sample_c; // RSSI sample
    uint8_t tail_c; // Tail message (ID)
    uint64_t tail_pips; // Tail pips
    uint64_t timeOut_c; // Timeout
}counter;


typedef struct repeater{
    bool ack_c; // Command ACK flag
    bool allow_c; // User control enable flag
    bool beacon_id;
    uint courtesy_freq;
    uint courtesy_select;
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
    bool tail_pips;
    uint64_t timeOut;
    bool tb; // Tone burst
    bool timeout_enabled;
    bool timedOut;
}rpt;