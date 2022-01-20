#define DEBUG

// ID
#define BEACON_ID //Uncomment to enable BEACON ID
#define CLOSE_DOWN_ID //Uncomment to enable ID on closedown
#define COURTESY_TONE_FREQ 1240
#define CW_BEACON_FREQ 2100 //CW tone frequency for periodic beacon
#define CW_CLOSEDOWN_FREQ 680 //CW tone frequency for closedown ID
#define dit 60
#define dah dit*3
#define space dah
char* morse[6] = {"--.","-...","...--","...-",".--", "-.."};

// IO
#define COS_INVERT //Uncomment to flip COS signal
#define CTCSS_DECODE_INVERT //Uncomment to flip CTCSS decode signal
#define DTMF_INVERT
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
#define HANGTIME 10000000ULL
#define ID 300000
#define LATCHTIME 2000000ULL
#define SAMPLETIME 2000000ULL
#define TIMEOUT 240000