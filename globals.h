// Options
#define DEBUG
#define DTMF_ALLSTAR_START 0xB
#define MODE 5 // RX mode. 0 Carrier only, 1 CTCSS or toneburst, 2 CTCSS, 3 Toneburst, 4 same as 0 with gateway, 5, same as 1 with gateway, 6 same as 2 with gateway, 7, same as 3 with gateway
#define USER_COMMAND 0

// ID
#define BEACON_ID //Uncomment to enable BEACON ID
//#define CLOSE_DOWN_ID //Uncomment to enable ID on closedown
#define COURTESY_TONE_FREQ 1300
#define CW_BEACON_FREQ 2100 //CW tone frequency for periodic beacon
#define CW_CLOSEDOWN_FREQ 680 //CW tone frequency for closedown ID
#define dit 64
#define dah dit*3
#define space dah
char* morse[6] = {"--.","-...","...--","...-",".--", "-.."};

// IO
#define COS_INVERT //Uncomment to flip COS signal
#define CTCSS_DECODE_INVERT //Uncomment to flip CTCSS decode signal
//#define DTMF_INVERT
//#define EXT_MUTE_INVERT//Uncomment to flip EXT_MUTE signal
//#define EXT_PTT_INVERT//Uncomment to flip EXT_PTT signal
#define EXT_RX_INVERT
//#define PTT_INVERT //Uncomment to flip PTT signal
//#define RF_MUTE_INVERT //Uncomment to flip RF_MUTE signal
#define TB_INVERT //Uncomment to flip Tone burst signal

// RSSI
#define RSSI_LOW 380 // RSSI LOW SIGNAL
#define RSSI_HIGH 440 // RSSI HIGH SIGNAL

// Time
#define HANGTIME 5000000ULL
#define ID 300000
#define LATCHTIME 2000000ULL
#define SAMPLETIME 2000000ULL
#define TIMEOUT 10000000ULL