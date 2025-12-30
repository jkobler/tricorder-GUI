#include <cstring>
#include <string.h>
#include "Adafruit_GFX.h"
#include "fonts/FreeSans9pt7b.h"
#include "fonts/FreeSansBold9pt7b.h"

#define DEBUG
//#define USE_ILI9341
#define USE_ST7789

#ifdef USE_ILI9341
#include "Adafruit_ILI9341.h"
#endif
#ifdef USE_ST7789
#include "Adafruit_ST7789.h"
#endif

typedef void (*FunctionCall)();

struct MenuItem {
    char *label;
    byte toMenuGroup;
    void (*toFunctionCall)();		
};

struct MenuGroup {
    byte id;
    char *label;
    const MenuItem *menuItems;
    byte itemCount;
    uint16_t activeColor;
    uint16_t inactiveColor;
    uint16_t activeTextColor;
    uint16_t inactiveTextColor;
};




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


//volatile int8_t activeMenuGroup=0;
volatile int8_t activeMenuItem=0;

const uint16_t MENU_BLACK = 0x0000;
const uint16_t MENU_GREEN = 0x07E0;
const uint16_t MENU_SLATEBLUE = 0x1B94;
const uint16_t MENU_ORANGE = 0b1111110011100101;
const uint16_t MENU_LIMEGREEN = 0b1011111001000110;
const uint16_t MENU_MAGENTA = 0xF81F;
const uint16_t MENU_WHITE = 0xFFFF;
const uint16_t MENU_RED = 0xF800;
const uint16_t MENU_CYAN = 0x07FF;

#ifdef USE_ILI9341
void drawMenu(Adafruit_ILI9341& tftPtr, MenuGroup *activeMenuGroup, int screenWidth, int screenHeight) {
#endif
#ifdef USE_ST7789
void drawMenu(Adafruit_ST7789& tftPtr, MenuGroup *activeMenuGroup, int screenWidth, int screenHeight) {
#endif
    int wip = screenWidth-20;
    int hip = screenHeight/10;
    int mx = 15;
    int my = hip+9;

    mx += 10;
    wip -= 10;
    for ( int i = 0; i<activeMenuGroup->itemCount; i++ ) {

        drawMenuItem(tftPtr, mx, my, wip, hip
            , activeMenuItem == i
            , activeMenuGroup->activeColor
            , activeMenuGroup->inactiveColor
            , activeMenuGroup->activeTextColor
            , activeMenuGroup->inactiveTextColor
            , activeMenuGroup->label);
        //yield();
        my = my + (hip+4);

    }



}

#ifdef USE_ILI9341
void drawMenuItem(Adafruit_ILI9341& tftPtr, int x, int y, int wip, int hip 
    , bool isActive
    , uint16_t activeColor
    , uint16_t inactiveColor
    , uint16_t activeTextColor
    , uint16_t inactiveTextColor
    , char *label) {
#endif
#ifdef USE_ST7789
void drawMenuItem(Adafruit_ST7789& tftPtr, int x, int y, int wip, int hip
    , bool isActive
    , uint16_t activeColor
    , uint16_t inactiveColor
    , uint16_t activeTextColor
    , uint16_t inactiveTextColor
    , char *label) {
#endif
    uint16_t bgColor=inactiveColor;
    uint16_t fgColor=inactiveTextColor;
    tftPtr.setFont(&FreeSans9pt7b);



    if (isActive == true) {
        bgColor=activeColor;
        fgColor=activeTextColor;
        tftPtr.setFont(&FreeSansBold9pt7b);

    }
    

    tftPtr.fillRoundRect(x, y, wip, hip, hip/2, bgColor);
    yield();
    tftPtr.setCursor(x+10,y+(hip*.1)+14);
    tftPtr.setTextColor(fgColor,bgColor);
    //tft.setTextSize(2);
    tftPtr.println(label);
    yield();

    //tft.color565(0, i, 0)
}

void setActiveMenuItem(int8_t n) {
    activeMenuItem=n;    
}

int8_t getActiveMenuItem() {
    return activeMenuItem;
}