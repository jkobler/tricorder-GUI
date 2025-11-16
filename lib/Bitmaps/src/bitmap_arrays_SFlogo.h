//#define USE_PROGMEM

#ifdef USE_PROGMEM
    #include "pgmspace.h"
#endif

#include "stdint.h"
//typedef unsigned short uint16_t;

#ifdef USE_PROGMEM
extern const uint8_t starfleet_TOS_bright_red[] PROGMEM;
extern const uint8_t starfleet_TOS_dark_blue[] PROGMEM; 
extern const uint8_t starfleet_TOS_dark_red[] PROGMEM;  
extern const uint8_t starfleet_TOS_light_grey[] PROGMEM;
extern const uint8_t starfleet_TOS_medium_red[] PROGMEM;
extern const uint8_t starfleet_TOS_mustard[] PROGMEM;
extern const uint8_t starfleet_TOS_white[] PROGMEM;
extern const uint8_t starfleet_TOS_yellow[] PROGMEM;
#else
extern const uint8_t starfleet_TOS_bright_red[];
extern const uint8_t starfleet_TOS_dark_blue[]; 
extern const uint8_t starfleet_TOS_dark_red[];  
extern const uint8_t starfleet_TOS_light_grey[];
extern const uint8_t starfleet_TOS_medium_red[];
extern const uint8_t starfleet_TOS_mustard[];
extern const uint8_t starfleet_TOS_white[];
extern const uint8_t starfleet_TOS_yellow[];
#endif

extern const int bmArraySize;

extern uint16_t TOS_color_bright_red;
extern uint16_t TOS_color_dark_blue;
extern uint16_t TOS_color_dark_red;
extern uint16_t TOS_color_light_grey;
extern uint16_t TOS_color_medium_red;
extern uint16_t TOS_color_mustard;
extern uint16_t TOS_color_white;
extern uint16_t TOS_color_yellow;