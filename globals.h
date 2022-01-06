// ID
#define dit 70
#define dah dit*3
#define space dah
char* morse[6] = {"--.","-...","...--","...-",".--", "-.."};


// IO
#define COS_INVERT //Uncomment to flip COS signal
//#define PTT_INVERT //Uncomment to flip PTT signal

// Time
#define HANGTIME 7000000ULL
#define ID 600000
#define LATCHTIME 2000000ULL
#define SAMPLETIME 2000000ULL
#define TIMEOUT 240000