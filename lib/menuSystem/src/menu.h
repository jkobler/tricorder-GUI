#include <cstring>
#include <string.h>
#include "Adafruit_GFX.h"
#include "fonts/FreeSans9pt7b.h"
#include "fonts/FreeSansBold9pt7b.h"

//#define USE_ILI9341
#define USE_ST7789

#ifdef USE_ILI9341
#include "Adafruit_ILI9341.h"
#endif
#ifdef USE_ST7789
#include "Adafruit_ST7789.h"
#endif

typedef void (*FunctionCall)();
extern struct MenuItem;
extern struct MenuGroup;


#ifdef USE_ILI9341
void drawMenu(Adafruit_ILI9341& tftPtr, MenuGroup *activeMenuGroup, int screenWidth, int screenHeight);
void drawMenuItem(Adafruit_ILI9341& tftPtr, int x, int y, int wip, int hip 
    , bool isActive
    , uint16_t activeColor
    , uint16_t inactiveColor
    , uint16_t activeTextColor
    , uint16_t inactiveTextColor
    , char *label);
#endif
#ifdef USE_ST7789
void drawMenu(Adafruit_ST7789& tftPtr, MenuGroup *activeMenuGroup, int screenWidth, int screenHeight);
void drawMenuItem(Adafruit_ST7789& tftPtr, int x, int y, int wip, int hip
    , bool isActive
    , uint16_t activeColor
    , uint16_t inactiveColor
    , uint16_t activeTextColor
    , uint16_t inactiveTextColor
    , char *label);
#endif
void setActiveMenuItem(int8_t n);
int8_t getActiveMenuItem();

//extern volatile int8_t activeMenuGroup;
extern volatile int8_t activeMenuItem;

extern const uint16_t MENU_BLACK;
extern const uint16_t MENU_GREEN;
extern const uint16_t MENU_SLATEBLUE;
extern const uint16_t MENU_ORANGE;
extern const uint16_t MENU_LIMEGREEN;
extern const uint16_t MENU_MAGENTA;
extern const uint16_t MENU_WHITE;
extern const uint16_t MENU_RED;
extern const uint16_t MENU_CYAN;