#define COS 5
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define DD 4
#define CTCSS_DETECT 6
#define CTCSS_ENCODE 10
#define EXT_RX 7
#define EXT_PTT 8
#define LAMP_TX 18
#define LAMP_SQL 19
#define LAMP_CTCSS_DECODE 20
#define LAMP_TB_DECODE 21
#define LAMP_CWID 22
#define RF_MUTE 9
#define EXT_MUTE 14
#define PTT 11
#define PIP 12
#define PTT_LED 25
#define RSSI 26
#define TONE_BURST 13

//MASKS
#define DTMF_MASK 0xF

int pins_num[22] = {
    D0,
    D1,
    D2,
    D3,
    DD,
    COS,
    CTCSS_DETECT,
    CTCSS_ENCODE,
    EXT_RX,
    EXT_PTT,
    LAMP_TX,
    LAMP_SQL,
    LAMP_CTCSS_DECODE,
    LAMP_TB_DECODE,
    LAMP_CWID,
    RF_MUTE,
    EXT_MUTE,
    PTT,
    PIP,
    PTT_LED,
    RSSI,
    TONE_BURST
};

int dir_num[22][2] = {
    {D0, GPIO_IN},
    {D1, GPIO_IN},
    {D2, GPIO_IN},
    {D3, GPIO_IN},
    {DD, GPIO_IN},
    {COS, GPIO_IN},
    {CTCSS_DETECT, GPIO_IN},
    {CTCSS_ENCODE, GPIO_OUT},
    {EXT_RX, GPIO_IN},
    {EXT_PTT, GPIO_OUT},
    {LAMP_TX, GPIO_OUT},
    {LAMP_SQL, GPIO_OUT},
    {LAMP_CTCSS_DECODE, GPIO_OUT},
    {LAMP_TB_DECODE, GPIO_OUT},
    {LAMP_CWID, GPIO_OUT},
    {EXT_MUTE, GPIO_OUT},
    {RF_MUTE, GPIO_OUT},
    {PTT, GPIO_OUT},
    {PIP, GPIO_OUT},
    {PTT_LED, GPIO_OUT},
    {RSSI, GPIO_IN},
    {TONE_BURST, GPIO_IN}
};