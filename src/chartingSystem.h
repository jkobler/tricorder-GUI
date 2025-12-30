#ifndef CHARTINGSYSTEM_H
#define CHARTINGSYSTEM_H
//#define USE_ILI9341
#define USE_ST7789
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <Adafruit_GFX.h>
#ifdef USE_ILI9341
#include <Adafruit_ILI9341.h>
#endif
#ifdef USE_ST7789
#include <Adafruit_ST7789.h>
#endif
#include "fonts/FreeSans9pt7b.h"
#include "fonts/FreeSansBold9pt7b.h"
#include "fonts/FreeSans12pt7b.h"
#include "fonts/FreeSansBold12pt7b.h"
#include "fonts/FreeMono9pt7b.h"
// Header file for chartingSystem
class Chart; 


#ifdef USE_ILI9341
void drawBarGraph(Adafruit_ILI9341&, Chart&, uint16_t, uint16_t, uint16_t);
void drawLineGraph(Adafruit_ILI9341&, Chart&, uint16_t, uint16_t, uint16_t);
void drawPieGraph(Adafruit_ILI9341&, Chart&, uint16_t, uint16_t, uint16_t);
void drawPiePointGraph(Adafruit_ILI9341&, Chart&, uint16_t, uint16_t, uint16_t);
typedef void (*ChartFunctionCall)(Adafruit_ILI9341&, Chart&, uint16_t, uint16_t, uint16_t);
#endif

#ifdef USE_ST7789
void drawBarGraph(Adafruit_ST7789&, Chart&, uint16_t, uint16_t, uint16_t);
void drawLineGraph(Adafruit_ST7789&, Chart&, uint16_t, uint16_t, uint16_t);
void drawPieGraph(Adafruit_ST7789&, Chart&, uint16_t, uint16_t, uint16_t);
void drawPiePointGraph(Adafruit_ST7789&, Chart&, uint16_t, uint16_t, uint16_t);
typedef void (*ChartFunctionCall)(Adafruit_ST7789&, Chart&, uint16_t, uint16_t, uint16_t);
#endif

typedef struct {
    std::string name;
    ChartFunctionCall toChartFunctionCall;		
} ChartType;


class Chart {
    class DataPoint {
    public: 
        std::string name;
        int32_t valueX;
        int32_t valueY;
        DataPoint(std::string cname,int32_t cvalueX,int32_t cvalueY);
        std::string getName();
        int32_t getValueX();
        int32_t getValueY();
        void setName(std::string cname);
        void setValueX(int32_t cvalueX);
        void setValueY(int32_t cvalueY);
        bool operator<(const DataPoint& other) const;
    };
    class DataGroup {
    public:
        std::string name;
        int32_t rangeLow;
        int32_t rangeHigh;
        std::string nameX;
        std::string nameY;
        std::vector<DataPoint> dataPoints;
        std::map<std::string,int> dataPointsIndex;
        DataGroup(std::string cname, int32_t crangeLow, int32_t crangeHigh, std::string cnameX, std::string cnameY);
        void addDataPoint(std::string cname,int32_t cvalueX,int32_t cvalueY);
        int getDataPointCount();
        DataPoint getDataPoint(std::string dataPointName);
        DataPoint getDataPointAt(int idx);
        bool operator<(const DataGroup& other) const;

    };
public:

    static const uint8_t BAR_CHART = 1;
    static const uint8_t LINE_CHART = 2;
    static const uint8_t PIE_CHART = 3;
    static const uint8_t PIEPOINT_CHART = 4;
    const ChartType chartTypes[4] = {
        {"bar", drawBarGraph},
        {"line", drawLineGraph},
        {"pie", drawPieGraph},
        {"piePoint", drawPiePointGraph}
    };


    std::string title;
    std::vector<DataGroup> dataGroups;
    std::map<std::string,int> dataGroupsIndex;
    uint8_t type;

    Chart(std::string chartTitle,uint8_t chartType);
    std::string getTitle();
    void setTitle(std::string chartTitle );
    void setChartType(uint8_t ctype);
    uint8_t getChartType();
    std::string getChartTypeName();
    void addDataGroup(std::string name, int32_t rangeLow,int32_t rangeHigh,std::string nameX,std::string nameY);
    void addDataToGroup(std::string name, std::string dataPointName, int32_t cvalueX,int32_t cvalueY);
    DataGroup getDataGroup(std::string dataGroupName);
    DataGroup getDataGroupAt(int idx);
}; 

Chart setChartTestData();






#endif // CHARTINGSYSTEM_H