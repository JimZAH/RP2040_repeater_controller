#define COS 5
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define DD 4
#define CTCSS 6
#define EXT_RX 7
#define EXT_PTT 8
#define RF_MUTE 9
#define EXT_MUTE 10
#define PTT 11
#define PIP 12
#define PTT_LED 25
#define RSSI 26
#define TONE_BURST 13

#define GPIO_BASE 0xd0000010;

//FFFFFFF0
//FFFFFFF

int pins_num[16] = {
    D0,
    D1,
    D2,
    D3,
    DD,
    COS,
    CTCSS,
    EXT_RX,
    EXT_PTT,
    RF_MUTE,
    EXT_MUTE,
    PTT,
    PIP,
    PTT_LED,
    RSSI,
    TONE_BURST
};

int dir_num[16][2] = {
    {D0, GPIO_IN},
    {D1, GPIO_IN},
    {D2, GPIO_IN},
    {D3, GPIO_IN},
    {DD, GPIO_IN},
    {COS, GPIO_IN},
    {CTCSS, GPIO_IN},
    {EXT_RX, GPIO_IN},
    {EXT_PTT, GPIO_OUT},
    {EXT_MUTE, GPIO_OUT},
    {RF_MUTE, GPIO_OUT},
    {PTT, GPIO_OUT},
    {PIP, GPIO_OUT},
    {PTT_LED, GPIO_OUT},
    {RSSI, GPIO_IN},
    {TONE_BURST, GPIO_IN}
};