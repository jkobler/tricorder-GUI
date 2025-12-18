

#define DEBUG
//#define HAS_RTC
//#define USE_SD_CARD
#define METRO_ESP32_S2
//#define FEATHER_ESP32_S2
#define USE_BITMAP_ARRAY
//#define USE_PROGMEM_FOR_BITMAP_ARRAY
#define USE_I2C
#define USE_ILI9341
//#define USE_ST7789

#include <cstring>
#include <string.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#ifdef USE_ILI9341
  #include "Adafruit_ILI9341.h"
#endif

#ifdef USE_ST7789
  #include "Adafruit_ST7789.h"
#endif

#ifdef USE_SD_CARD
  #include "SdFat.h"
  #include "SdCard/SdCardInfo.h"
#endif

#include "Adafruit_ImageReader.h"
#include "fonts/FreeSans9pt7b.h"
#include "fonts/FreeSansBold9pt7b.h"
#include "fonts/FreeSans12pt7b.h"
#include "fonts/FreeSansBold12pt7b.h"
#include "fonts/FreeMono9pt7b.h"
#include "Fonts/FreeSerif9pt7b.h"
#include "Fonts/FreeSerifItalic9pt7b.h"
#include "pgmspace.h"

#ifdef USE_BITMAP_ARRAY
  #include "bitmap_arrays_SFlogo.h"
#endif

#ifdef HAS_RTC
  #include "RTC.h"
#endif

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#define GRAY 0xBBBB

/*
COLORS:
  16 bit values 
  0-4 = Blue
  5-10 = Green
  11-15 = Red

TOS pallet.

#fdcd06	(253,205,6)    11111 110011 00001
#b9c92f	(185,201,47)   10111 110010 00110
#bf0f0f	(191,15,15)    11000 000100 00010
#165fc5	(22,95,197)    00011 011000 11000
#11709f	(17,112,159)   00011 011100 10100

From scanner pic
#756643 limeGreen
#ff9b25 orange
#eece2d yellow
#eb4035 red
font                 xAdvance  yAdvance

FreeMono9pt7b        11        18
FreeMonoBold9pt7b    11        18
FreeMono12pt7b       14        24
FreeMonoBold12pt7b   14        24
FreeMono18pt7b       21        35
FreeMonoBold18pt7b   21        35
FreeMono24pt7b       28        47
FreeMonoBold24pt7b   28        47

FreeSans9pt7b        5..18     22
FreeSansBold9pt7b    5..18     22
FreeSans12pt7b       6..24     29
FreeSans18pt7b       9..36     42
FreeSans24pt7b       12..48    56

FreeSerif9pt7b       5..17     22

*/

#define TOS_YELLOW 0b1110100011000110
#define TOS_LIMEGREEN 0b1011111001000110
#define TOS_RED 0b1110101000000111
#define TOS_ORANGE 0b1111110011100101
#define TOS_BLUE 0x1B18
#define TOS_SLATEBLUE 0x1B94

#define PINK 0b1111111100011000

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 12
//#define SD_RST 12

void loop();
void drawMenuItem(int, int, int, int, byte);
void drawMenu();
void handleRotaryEncoder();
void handleSwitch1();
void handleSwitch2();
void handleSwitch3();
void handleSwitch4();
void constantLights(byte,byte);
void blinkyLights(byte, int);
void drawLogoBitmap();
void drawLogoBitmap(uint32_t delayTime);
void openAudioScreen();
void openVideoScreen();
void printCenterJustifiedText(const char*, u_int16_t, bool, uint16_t, uint16_t);
void printPaginatedText(const char*,uint16_t,uint16_t);
void openReaderScreen();

#ifdef HAS_RTC
  void drawHeader(RTCTime);
  float StardateConverter(RTCTime, int);
#else
   void drawHeader();
#endif

#ifdef USE_SD_CARD
  SdFat                SD;         // SD card filesystem
  Adafruit_ImageReader reader(SD); // Image-reader object, pass in SD filesys
#endif

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


typedef uint16_t Color;
typedef void (*FunctionCall)();


struct MenuItem {
  char label[25];
  byte toMenuGroup;
  FunctionCall toFunctionCall;		
};


struct MenuGroup {
  byte id;
  char label[20];
  struct MenuItem menuItems[20];
  byte itemCount;
  Color activeColor;
  Color inactiveColor;
  Color activeTextColor;
  Color inactiveTextColor;
};


const struct MenuGroup menuGroups[6] = {
  { 0,"Home",{{"Media",1},{"Information",2},{"Sensors",3},{"Turbo encabulator",4},{"Settings",5}},5,TOS_ORANGE,TOS_LIMEGREEN,BLACK,BLACK },
  { 1,"Media",{{"Back",0},{"Audio",0,openAudioScreen},{"Images",0,drawLogoBitmap},{"Video",0,openVideoScreen},{"Reader",0,openReaderScreen}},5,TOS_LIMEGREEN,GREEN,BLACK,BLACK },
  { 2,"Information",{{"Back",0},{"Lookup",0},{"Download",0},{"Upload",0}},4,CYAN,TOS_SLATEBLUE,BLACK,WHITE },
  { 3,"Sensors",{{"Back",0},{"Perimeter",0},{"Environmental",0}},3,TOS_ORANGE,RED,BLACK,WHITE },
  { 4,"Turbo encabulator",{{"Back",0},{"Unilateral Detractors",0},{"Synchronize Grammeter",0},{"HF Rem Peak Setting",0},{"Write-only Memory",0}},5,MAGENTA,TOS_SLATEBLUE,WHITE,WHITE },
  { 5,"Settings",{{"Back",0},{"Menu Item 42",0,drawLogoBitmap},{"Menu Item 43",0}},3,TOS_ORANGE,TOS_LIMEGREEN,BLACK,BLACK }
};

volatile byte activeMenuGroup=0;
volatile byte activeMenuItem=0;
volatile bool needsRefresh=1;
volatile bool onFunctionScreen=0;
volatile int loopCount = 0;


byte menuGroupCount = 5; //because sizeof is problematic

int screenWidth;
int screenHeight;

byte switch1 = 7; 
byte switch2 = 8; 
//byte encoder_ = 6; //orange
//byte switch4 = 5; //blue
byte leds[3] = {13,14,15};




volatile int prevRotations = 0;
volatile bool switch1State = 0;
volatile bool switch2State = 0;
volatile int rotations = 0;
volatile int lastMinutes = 0;

void setup() {

  ImageReturnCode stat; // Status from image-reading functions
  Serial.begin(115200);

  pinMode(switch1,INPUT_PULLUP);
  pinMode(switch2,INPUT_PULLUP);
  //pinMode(switch3,INPUT_PULLUP);
  //pinMode(switch4,INPUT_PULLUP);
  //pinMode(rotaryDt,INPUT);
  //pinMode(rotaryClk,INPUT_PULLUP);
  pinMode(leds[0],OUTPUT);
  pinMode(leds[1],OUTPUT);
  pinMode(leds[2],OUTPUT);


#ifdef HAS_RTC
  RTC.begin();
  //RTCTime startTime(dayInt, getMonthFromInt(monthInt), 2366, hourInt, minuteInt, 00, getDayOfWeekFromString(dayOfWeekStr), isDSTActive(isDSTActiveStr));
  RTCTime startTime(7, Month::JULY, 2266, 8, 0, 0, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_ACTIVE);
 
  RTC.setTime(startTime);
#endif

  tft.begin();
  tft.setRotation(1);
  tft.setFont(&FreeSansBold12pt7b);
  

#ifdef DEBUG
  Serial.println("TC25 Operating System\nVer 20251108a");
  delay(200);
#endif
  while (!Serial) {
  }
  delay(1000);
  screenWidth = tft.width();
  screenHeight = tft.height();

  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);
  tft.setCursor(5, 17);
  tft.println("TC25 Operating System");
  tft.println("    Ver 20251108a");
  tft.setTextColor(BLUE);
  tft.println("  Turbo encabulator ");
  tft.println("      Edition");
  delay(2000);

#ifdef USE_SD_CARD
  if(!SD.begin(SD_CS)) { 
    Serial.println(F("SD begin() failed"));
    tft.fillScreen(BLACK);
    tft.setFont(&FreeSansBold9pt7b);
    tft.setCursor(1, 14);
    tft.setTextColor(RED,BLACK);    
    tft.println("SD begin() failed");
    printSdErrorText(&Serial,SD.sdErrorCode());
    printSdErrorText(&tft,SD.sdErrorCode());
    delay(1000);

    for(;;); // Fatal error, do not continue
  }
  Serial.println("SDCard initialization done.");
  tft.println("SDCard initialization done.");
#endif




  delay(1000);
  tft.fillScreen(BLACK);

  for(int i = 0; i<11;i++) {
    blinkyLights(i%3, 300);
  }
  // /pics/Tricorder Screen 1.bmp
  // /pics/starfleet_TOS-2_black_bg.bmp

#ifdef USE_SD_CARD
  stat = reader.drawBMP("/pics/SF_TOS-1.bmp", tft, 0, 0);

  reader.printStatus(stat);   // How'd we do?
  #ifdef DEBUG
  switch (stat) {
    case IMAGE_ERR_FILE_NOT_FOUND:
      Serial.println("SF_TOS-1.bmp:Could not open the requested file");
      break;
    case IMAGE_ERR_FORMAT :
      Serial.println("SF_TOS-1.bmp:Not a supported image format");
      break;
    case IMAGE_ERR_MALLOC :
      Serial.println("SF_TOS-1.bmp:Could not allocate memory for operation");
      break;
    default:
      Serial.println("SF_TOS-1.bmp:Image loaded successfully");
  } 
  #endif   

      
#endif


#ifdef USE_BITMAP_ARRAY
  drawLogoBitmap();
#endif

#ifdef DEBUG
  Serial.println("Starting firmware");
  Serial.print("screenWidth:");
  Serial.println(screenWidth);
  Serial.print("screenHeight:");
  Serial.println(screenHeight);


#endif

  delay(2000);
  // attachInterrupt(switch3, handleSwitch3, FALLING);
  // attachInterrupt(switch4, handleSwitch4, FALLING);
  attachInterrupt(switch1, handleSwitch1, FALLING);
  attachInterrupt(switch2, handleSwitch2, FALLING);


}

void loop() {
#ifdef HAS_RTC  
    RTCTime currentTime;
    RTC.getTime(currentTime);
  int minutes = currentTime.getMinutes() + (currentTime.getHour()*60);
  if (lastMinutes != minutes) {
    needsRefresh = HIGH;
    lastMinutes = minutes;
  }
#endif
    
  if (prevRotations != rotations) {
#ifdef DEBUG      
    Serial.print("loop(): prevRotations: ");
    Serial.print(prevRotations);
    Serial.print("; rotations: ");
    Serial.println(rotations);
  //  delay(100);
#endif

    needsRefresh=true;
    prevRotations = rotations; 
    activeMenuItem = abs(rotations)%menuGroups[activeMenuGroup].itemCount;
    Serial.print("loop(): ");
    Serial.print("activeMenuItem:");
    Serial.println(activeMenuItem);
    delay(200);
    

  } //if (rotaryClkVal != rotaryClkLastVal && rotaryClkVal == 0)

  if (switch1State == true) {
    switch1State = false;
  
#ifdef DEBUG    
    Serial.print("loop(): ");
    Serial.println(menuGroups[activeMenuGroup].menuItems[activeMenuItem].label);
#endif 

    if (onFunctionScreen==true) {
      onFunctionScreen = false;
      needsRefresh=true;
#ifdef DEBUG  
      Serial.print(" is selected. Leaving screen for MenuGroup ");
      Serial.println(menuGroups[activeMenuGroup].menuItems[activeMenuItem].toMenuGroup);
#endif 
    }
    else if (menuGroups[activeMenuGroup].menuItems[activeMenuItem].toFunctionCall == nullptr ) {
      activeMenuGroup = menuGroups[activeMenuGroup].menuItems[activeMenuItem].toMenuGroup;
      activeMenuItem = 0;
      needsRefresh=true;
#ifdef DEBUG  
      Serial.print(" is selected. This calls MenuGroup ");
      Serial.println(menuGroups[activeMenuGroup].menuItems[activeMenuItem].toMenuGroup);
#endif 
    }
    else {
      menuGroups[activeMenuGroup].menuItems[activeMenuItem].toFunctionCall();
      onFunctionScreen=true;
      activeMenuItem = 0;
      needsRefresh=false;
#ifdef DEBUG  
      Serial.print(" is selected. This calls a screen ");
#endif 
    }

  }

  if (switch2State == true) {
    switch2State = false;
    needsRefresh=true;  
    if (activeMenuGroup > 0 && onFunctionScreen==false) 
      activeMenuGroup--;
    activeMenuItem = 0; //depends on first group being back button
    onFunctionScreen=false;

#ifdef DEBUG  
    Serial.print("loop(): ");
    Serial.println("going back");
#endif 
  }

  

  if (needsRefresh == true) {
#ifdef HAS_RTC
    drawHeader(currentTime);
#else
    drawHeader();
#endif
    drawMenu();
    needsRefresh = false;

#ifdef DEBUG      
    Serial.print("loop(): ");
    Serial.println(menuGroups[activeMenuGroup].menuItems[activeMenuItem].label);
#endif    

  //  delay(100);
  }


}

void handleRotaryEncoder() {
    rotations++;
}
// void handleSwitch3() {
//     rotations++;
// }
// void handleSwitch4() {
//     rotations--;
// }

void handleSwitch1() {
  switch1State = true;
}

void handleSwitch2() {
  switch2State = true;
}

#ifdef HAS_RTC
void drawHeader(RTCTime currentTime) {
    int mx = 5;
    int my = 5;
    // byte wip = screenWidth-20;
    // byte hip = screenHeight/10;
    int wip = screenWidth-20;
    int hip = screenHeight/10;
    float starDate = StardateConverter(currentTime,0);
#ifdef DEBUG
    Serial.print("starDate:");
    Serial.println(starDate);

#endif
    tft.fillScreen(ILI9341_BLACK);
    //yield();
    tft.setFont(&FreeSansBold9pt7b);
    tft.fillRoundRect(mx, my, wip, hip, hip/2, menuGroups[activeMenuGroup].inactiveColor);
    yield();
    tft.setCursor(mx+10,my+(hip*.1)+14);
    tft.setTextColor(menuGroups[activeMenuGroup].inactiveTextColor,menuGroups[activeMenuGroup].inactiveColor);
    //tft.setTextSize(2);
    
    tft.print(menuGroups[activeMenuGroup].label);
    tft.print("  -  ");
    tft.println(starDate);
    yield();
}
#else
void drawHeader() {
    int mx = 5;
    int my = 5;
    // byte wip = screenWidth-20;
    // byte hip = screenHeight/10;
    int wip = screenWidth-20;
    int hip = screenHeight/10;

    tft.fillScreen(ILI9341_BLACK);
    //yield();
    tft.setFont(&FreeSansBold9pt7b);
    tft.fillRoundRect(mx, my, wip, hip, hip/2, menuGroups[activeMenuGroup].inactiveColor);
    yield();
    tft.setCursor(mx+10,my+(hip*.1)+14);
    tft.setTextColor(menuGroups[activeMenuGroup].inactiveTextColor,menuGroups[activeMenuGroup].inactiveColor);
    //tft.setTextSize(2);
    
    tft.print(menuGroups[activeMenuGroup].label);

    yield();
}
#endif

void drawMenu() {
    int wip = screenWidth-20;
    int hip = screenHeight/10;
    int mx = 15;
    int my = hip+9;

#ifdef DEBUG
      //char buffer[100];
      //sprintf(buffer,"activeMenuGroup: %d; menuItem: %d; mx: %d; my: %d; wip: %d; hip: %d",activeMenuGroup,i,mx,my,wip,hip );
      //Serial.println(buffer);
      Serial.print("drawMenu(): screenWidth:");
      Serial.println(screenWidth);
      Serial.print("drawMenu(): screenHeight:");
      Serial.println(screenHeight);
      Serial.print("drawMenu(): activeMenuGroup:");
      Serial.print(activeMenuGroup);
      Serial.print("; wip:");
      Serial.print(wip);
      Serial.print("; hip:");
      Serial.println(hip);
#endif
    mx += 10;
    wip -= 10;
    for ( int i = 0; i<menuGroups[activeMenuGroup].itemCount; i++ ) {

#ifdef DEBUG
      //char buffer[100];
      //sprintf(buffer,"activeMenuGroup: %d; menuItem: %d; mx: %d; my: %d; wip: %d; hip: %d",activeMenuGroup,i,mx,my,wip,hip );
      //Serial.println(buffer);
      // Serial.print("screenWidth:");
      // Serial.println(screenWidth);
      // Serial.print("screenHeight:");
      // Serial.println(screenHeight);

      Serial.print("drawMenu(): menuItem:");
      Serial.print(i);
      Serial.print("; mx:");
      Serial.print(mx);
      Serial.print("; my:");
      Serial.println(my);
#endif

      drawMenuItem(mx, my, wip, hip, i);
      //yield();
      my = my + (hip+4);

    }



}


void drawMenuItem(int x, int y, int wip, int hip, byte menuItemId) {
  
  uint16_t bgColor=menuGroups[activeMenuGroup].inactiveColor;
  uint16_t fgColor=menuGroups[activeMenuGroup].inactiveTextColor;
  tft.setFont(&FreeSans9pt7b);



  if (menuItemId == activeMenuItem) {
    bgColor=menuGroups[activeMenuGroup].activeColor;
    fgColor=menuGroups[activeMenuGroup].activeTextColor;
    tft.setFont(&FreeSansBold9pt7b);

  }
  

  tft.fillRoundRect(x, y, wip, hip, hip/2, bgColor);
  yield();
  tft.setCursor(x+10,y+(hip*.1)+14);
  tft.setTextColor(fgColor,bgColor);
  //tft.setTextSize(2);
  tft.println(menuGroups[activeMenuGroup].menuItems[menuItemId].label);
  yield();

  //tft.color565(0, i, 0)
}


void printCenterJustifiedText(const char* text, u_int16_t cy, bool isBold, uint16_t fgColor, uint16_t bgColor) {
  int16_t x1, y1;
  uint16_t w, h;

  tft.setTextColor(fgColor,bgColor);

  if (isBold == true)
    tft.setFont(&FreeSansBold9pt7b);
  else
    tft.setFont(&FreeSans9pt7b);
  
  tft.getTextBounds(text, 0, cy, &x1, &y1, &w, &h);
  int16_t startX = (screenWidth - w) / 2;


  tft.setCursor(startX,y1);
  tft.println(text);
}

void printPaginatedText(const char* text, uint16_t fgColor, uint16_t bgColor ) {

  //int pageNumber = 0;
  //int lastChar = 0;
  int16_t x1, y1;
  uint16_t w, h;
  //int maxCharsPerScreen = 0;
  int textLength = 0;
  byte linesPerScreen =13;
  //int avgCharWidth=0;
  byte charsPerLine = 38;
  bool stillReading = true;
  


  //for scrolling... 
  /*
    1. Determine text length (size HxW) for desired font.
    2. breakup the text into lines
    3. write only the lines that will fit based on scrol; cursor position

    change of plans
    WITH FreeSans9pt7b...
    16 characters per line 
    11 lines per screen.
  */

  tft.fillScreen(bgColor);
  tft.setTextColor(fgColor,bgColor);
  //                                xAdv   yAdv
  //tft.setFont(&FreeSans9pt7b); // 5-18     22   7
  tft.setFont(&FreeMono9pt7b); //   11     18   11
 // tft.setFont(&FreeSerif9pt7b); // 5-17     22    7
  textLength = strlen(text);
  //tft.getTextBounds("the quick, brown fox jumped over the lazy dog. THE QUICK, BROWN FOX JUMPED OVER THE LAZY DOG!", 0, 0, &x1, &y1, &w, &h);

  charsPerLine = screenWidth/11;
 //charsPerLine = 40;



  // maxCharsPerScreen = linesPerScreen * charsPerLine;

  //setup the text for proper formatting...
  int i=0,j=0,ch=0,lastSpaceInLine=0,lineCount=0,estTotalLines=((textLength/charsPerLine)*1.5)+3, chOffset = 0;
  //char lineBuffer[charsPerLine+1];
  char formattedText[estTotalLines][charsPerLine+1];

  #ifdef DEBUG
    Serial.print(" linesPerScreen: ");
    Serial.print(linesPerScreen);
    // Serial.print(" test text width: ");
    // Serial.print(w);
    // Serial.print(" Average Char width (w/93): ");
    // Serial.print((w/93));
    Serial.print("; charsPerLine: ");
    Serial.print(charsPerLine);
    Serial.print("; textLength: ");
    Serial.print(textLength);
    Serial.print("; estTotalLines: ");
    Serial.println(estTotalLines);
  #endif

  for (lineCount = 0; lineCount < estTotalLines && ch < textLength; lineCount++) {
      constantLights(0,1);

    for (i=0; i < charsPerLine && ch < textLength; i++) {
      constantLights(1,1);

      formattedText[lineCount][i] = text[ch];
      // if (text[ch] == ' ' || text[ch]  == '.' || text[ch]  == ',' || text[ch]  == ';' 
      //   || text[ch]  == ':' || text[ch]  == '!' || text[ch]  == '\n' || text[ch]  == '?')
      //   lastSpaceInLine = i;
      if (text[ch] == ' ' || text[ch]  == '\n' || text[ch]  == '-')
        lastSpaceInLine = i;
        ch++;
    } //for (i=0; i < charsPerLine && ch < (textLength-1); i++)
    constantLights(1,0);
    constantLights(2,1);

/*
    scenarios                                                                     lastSpaceInLine                ch
      1. We've transversed the whole line (i = 38), we are still ch<textLength, 
         and there is a white space character before that but past 60% (23)       The last space encountered;    back to last white space.
      2. We've hit the end of the text (ch >= textLength)                         i-1;                           no change
      3. We've transversed the whole line (i = 38), we are still ch<textLength,   i-1;                           no change
         there is either no whitespace before 60%, but there is whitespace the 
         next character up.
      4. We've transversed the whole line (i = 38), we are still ch<textLength,   i-1;                           no change
         there is either no whitespace before 60%.
*/
    chOffset = 1;
    if ((ch < textLength  
       && (text[ch] == ' ' || text[ch]  == '\n' || text[ch]  == '-'))
      || ch >= textLength ) {
      lastSpaceInLine = i;  
    //  chOffset = -1;
#ifdef DEBUG
      Serial.print("White Space or end of text."); 
#endif
    }
#ifdef DEBUG
      Serial.print("; ch before offset: ");
      Serial.print(ch);
#endif

    if (lastSpaceInLine > 1 && ch < textLength) { 
      ch -= (charsPerLine - lastSpaceInLine)-chOffset;
      
    }

#ifdef DEBUG
      Serial.print("; lastSpaceInLine: ");
      Serial.print(lastSpaceInLine);
      Serial.print("; chOffset:" ); 
      Serial.print(chOffset); 
      Serial.print("; ch: ");
      Serial.println(ch);
#endif

    formattedText[lineCount][lastSpaceInLine] = '\0';
    
//    strncpy(formattedText[lineCount++],lineBuffer,lastSpaceInLine+1);
      //formattedText[lineCount++] = lineBuffer;
#ifdef DEBUG
      Serial.print("; formattedText[");
      Serial.print(lineCount);
      Serial.print("]: \"");
      Serial.print(formattedText[lineCount]);
      Serial.println("\"");
#endif
    constantLights(2,0);
    constantLights(0,0);

    lastSpaceInLine = 0;
  } //for (int ch=0; ch<textLength; ch++) {

  //char sectionBuffer[maxCharsPerScreen];
  switch1State=HIGH;
  switch2State=LOW;
  lastSpaceInLine = 0; //reusing this. It's now the last line printed
  tft.setRotation(1);
//  byte lightIdx = 0;
#ifdef DEBUG
      Serial.print("; lineCount");
      Serial.println(lineCount);
#endif
  while (stillReading) {
    constantLights(0,1);

    if (switch1State == HIGH) {
      constantLights(1,1);
      if (lastSpaceInLine < lineCount) {
        tft.fillScreen(bgColor);
        tft.setCursor(1,14);
        tft.setTextColor(fgColor,bgColor);
#ifdef DEBUG
        Serial.print("Turning the page forward: ");
        Serial.println(lastSpaceInLine);        
#endif
        for (i=0;lastSpaceInLine+i<lineCount && i<linesPerScreen; i++ ) {
          tft.println(formattedText[lastSpaceInLine+i]);
#ifdef DEBUG
          Serial.print("formattedText[");
          Serial.print(lastSpaceInLine+i);
          Serial.print("] = \"");
          Serial.print(formattedText[lastSpaceInLine+i]);
          Serial.println("\"");
#endif
        }
        lastSpaceInLine = i;

      }
      constantLights(1,0);
      switch1State=LOW;
    } //if (switch1State == HIGH)
    else if (switch2State == HIGH) {
      constantLights(2,1);
      if (lastSpaceInLine > linesPerScreen) {
        tft.fillScreen(bgColor);
        tft.setCursor(1,14);
        tft.setTextColor(fgColor,bgColor);    
        lastSpaceInLine -= linesPerScreen;   
#ifdef DEBUG
        Serial.print("Turning the page backward: ");
        Serial.println(lastSpaceInLine);        
#endif        
        for (i=0;lastSpaceInLine+i<lineCount && i<linesPerScreen; i++ ) {
          tft.println(formattedText[lastSpaceInLine+i]);
#ifdef DEBUG
          Serial.print("formattedText[");
          Serial.print(lastSpaceInLine+i);
          Serial.print("] = \"");
          Serial.print(formattedText[lastSpaceInLine+i]);
          Serial.println("\"");
#endif
        }
        lastSpaceInLine = i;
      }
      else {
        stillReading = false;
      }
      switch2State=LOW;
      constantLights(2,0);
    } // else if (switch2State == HIGH) 
    delay(10);
  } //while (stillReading)
  constantLights(0,0);
  constantLights(1,0);
  constantLights(2,0);
  onFunctionScreen=false;
  needsRefresh = true;
}



#ifdef HAS_RTC
  /*
    1 year = 1000 Stardate Units (SU) = y
    Days of the year 365 | 366 = d
    base date 01-01-2005 = 58000.00  b,c
    base date 01-01-2323 = 00000.00  b,c

    c + (1000*(y-b)) + ((1000/n)*(m + d -1)) = Stardate

  */
float StardateConverter(RTCTime currentTime, int yearOffset) {
  int y = currentTime.getYear()+yearOffset;
  int m = Month2int(currentTime.getMonth())-1;
  //int daysPerSu = 8760/1000;
  int daysPerSu = 1000/365;
  int hours = currentTime.getHour();
  int minutes = currentTime.getMinutes();

  int gregOffsets[] = {0,31,59,90,120,151,181,212,243,273,304,334};
  //leap year offset
  for (int i=2;y%4 == 0&&i<12;i++) {
    gregOffsets[i]++;
    if (i==2) 
      //daysPerSu = 8784/1000;
      daysPerSu = 1000/366;
  }
  
  //xxxxx.x Right of the decimal is a faction of the day. There are 144 minutes per tenth of the day.
  float fractionOfDay = ((float) hours*60 + (float) minutes)/144.00;

  //base year 2005 by default; 
  float baseYear = 2005; 
  float baseStarDate = 58000.00;
  if (y >= 2323) {
    baseStarDate = 0;
    baseYear = 2323;
  }
   float gregSu = 0;
  // //because January is a div/0 generator.
   if (m > 0) 
     gregSu = (int) round((gregOffsets[m-1]+currentTime.getDayOfMonth()-1)/daysPerSu);

  #ifdef DEBUG
    Serial.print("hours:");
    Serial.print(hours);
    Serial.print("; minutes:");
    Serial.print(minutes);
    Serial.print("; gregSu:");
    Serial.print(gregSu);
    Serial.print("; fractionOfDay:");
    Serial.print(fractionOfDay);
    Serial.print("; baseYear:");
    Serial.print(baseYear);
    Serial.print("; y:");
    Serial.print(y);
    Serial.print("; m:");
    Serial.println(m);

  #endif
  
  return baseStarDate + (1000 * (y-baseYear)) + gregSu  +  (fractionOfDay/10);
}
#endif

void constantLights(byte idx,byte state) {
    digitalWrite(leds[idx], state);
}

void blinkyLights(byte idx,int delayTime) {
    digitalWrite(leds[idx], HIGH);
    delay(delayTime);
    digitalWrite(leds[idx], LOW);
//    yield();
}

#ifdef USE_BITMAP_ARRAY
void drawLogoBitmap() {
  drawLogoBitmap(5000);
}

void drawLogoBitmap(uint32_t delayTime) {

  tft.fillScreen(BLACK);

/*
const unsigned uint8_t  starfleet_TOS_bright_red  #FF0000    0b11111 111 000000 00 00000 000
const unsigned uint8_t  starfleet_TOS_dark_blue   #00007A    0b00000 000 000000 00 01111 010
const unsigned uint8_t  starfleet_TOS_dark_red    #920000    0b10010 010 000000 00 00000 000
const unsigned uint8_t  starfleet_TOS_light_grey  #DCDCDC    0b11011 100 110111 00 11011 100
const unsigned uint8_t  starfleet_TOS_medium_red  #DC0000    0b11011 100 000000 00 00000 000
const unsigned uint8_t  starfleet_TOS_mustard     #AAAA00    0b10101 010 101010 10 00000 000
const unsigned uint8_t  starfleet_TOS_white       #FFFFFF    0b11111 111 111111 11 11111 111
const unsigned uint8_t  starfleet_TOS_yellow      #FFFF00    0b11111 111 111111 11 00000 000
*/

  uint16_t x_offset = 0;
  #ifdef USE_PROGMEM_FOR_BITMAP_ARRAY
    uint8_t bitmapArray[bmArraySize] ;
    memcpy_P(bitmapArray, starfleet_TOS_bright_red, bmArraySize);
    tft.drawBitmap(0, 0, bitmapArray, 320, 240, TOS_color_bright_red);

    memcpy_P(bitmapArray, starfleet_TOS_dark_blue, bmArraySize);
    tft.drawBitmap(0, 0, bitmapArray, 320, 240, TOS_color_dark_blue);

    memcpy_P(bitmapArray, starfleet_TOS_dark_red, bmArraySize);
    tft.drawBitmap(0, 0, bitmapArray, 320, 240, TOS_color_dark_red);

    memcpy_P(bitmapArray, starfleet_TOS_light_grey, bmArraySize);
    tft.drawBitmap(0, 0, bitmapArray, 320, 240, TOS_color_light_grey);

    memcpy_P(bitmapArray, starfleet_TOS_medium_red, bmArraySize);
    tft.drawBitmap(0, 0, bitmapArray, 320, 240, TOS_color_medium_red);

    memcpy_P(bitmapArray, starfleet_TOS_mustard, bmArraySize);
    tft.drawBitmap(0, 0, bitmapArray, 320, 240, TOS_color_mustard);

    memcpy_P(bitmapArray, starfleet_TOS_white, bmArraySize);
    tft.drawBitmap(0, 0, bitmapArray, 320, 240, TOS_color_white);

    memcpy_P(bitmapArray, starfleet_TOS_yellow, bmArraySize);
    tft.drawBitmap(0, 0, bitmapArray, 320, 240, TOS_color_yellow);
  #else 
    tft.drawBitmap(x_offset, 0, starfleet_TOS_bright_red, 320, 240, TOS_color_bright_red);
    tft.drawBitmap(x_offset, 0, starfleet_TOS_dark_blue, 320, 240, TOS_color_dark_blue);
    tft.drawBitmap(x_offset, 0, starfleet_TOS_dark_red, 320, 240, TOS_color_dark_red);
    tft.drawBitmap(x_offset, 0, starfleet_TOS_light_grey, 320, 240, TOS_color_light_grey);
    tft.drawBitmap(x_offset, 0, starfleet_TOS_medium_red, 320, 240, TOS_color_medium_red);
    tft.drawBitmap(x_offset, 0, starfleet_TOS_mustard, 320, 240, TOS_color_mustard);
    tft.drawBitmap(x_offset, 0, starfleet_TOS_white, 320, 240, TOS_color_white);
    tft.drawBitmap(x_offset, 0, starfleet_TOS_yellow, 320, 240, TOS_color_yellow);
  #endif

  delay(delayTime);

}
#endif

// called functions

void openAudioScreen() {
  u_int16_t cy = screenHeight/2-14;
  tft.fillScreen(menuGroups[activeMenuGroup].activeColor);
  printCenterJustifiedText("Media - Audio", cy, true, menuGroups[activeMenuGroup].activeTextColor, menuGroups[activeMenuGroup].activeColor);
}

void openVideoScreen() {
  u_int16_t cy = screenHeight/2-14;
  tft.fillScreen(menuGroups[activeMenuGroup].activeColor);
  printCenterJustifiedText("Media - Video", cy, true, menuGroups[activeMenuGroup].activeTextColor, menuGroups[activeMenuGroup].activeColor);
}

void openReaderScreen() {
  char text[] = "For a number of years work has been proceeding in order to bring perfection to the crudely conceived idea of a machine that would not only supply inverse reactive current for use in unilateral phase detractors, but would also be capable of automatically synchronizing cardinal grammeters.  Such a machine is the Turbo-Encabulator. Basically, the only new principle involved is that instead of power being generated by the relative motion of conductors and fluxes, it is produced by the modial interaction of magneto-reluctance and capacitive directance.";
  //char text[] = "For a number of years work has been proceeding in order to bring perfection";

//  printPaginatedText( text, BLACK, ILI9341_LIGHTGREY ) ;
  printPaginatedText( text, ILI9341_DARKGREEN, BLACK ) ;
}


