#define COS 14
#define CTCSS 15
#define EXT_RX 13
#define EXT_PTT 12
#define RF_MUTE 11
#define EXT_MUTE 10
#define PTT 17
#define PIP 16
#define PTT_LED 25
#define RSSI 26

int pins_num[10] = {
    COS,
    CTCSS,
    EXT_RX,
    EXT_PTT,
    RF_MUTE,
    EXT_MUTE,
    PTT,
    PIP,
    PTT_LED,
    RSSI
};

int dir_num[10][1] = {
    {COS, GPIO_IN},
    {CTCSS, GPIO_IN},
    {EXT_RX, GPIO_IN},
    {EXT_PTT, GPIO_OUT},
    {EXT_MUTE, GPIO_OUT},
    {RF_MUTE, GPIO_OUT},
    {PTT, GPIO_OUT},
    {PIP, GPIO_OUT},
    {PTT_LED, GPIO_OUT},
    {RSSI, GPIO_IN}
};