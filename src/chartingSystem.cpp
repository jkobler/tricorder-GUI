

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
//using std::map;
using std::vector;
using std::string;

class Chart; 
Chart setChartTestData();


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
    string name;
    ChartFunctionCall toChartFunctionCall;		
} ChartType;




class Chart {


    class DataPoint {
    public:


        string name;
        int32_t valueX;
        int32_t valueY;

        DataPoint(string cname,int32_t cvalueX,int32_t cvalueY) {
            name = cname;
            valueX = cvalueX;
            valueY = cvalueY;
        }
        
        string getName() {
            return name;
        }

        int32_t getValueX() {
            return valueX;
        }

        int32_t getValueY() {
            return valueY;
        }

        void setName(string cname) {
            name = cname;          
        }
        void setValueX(int32_t cvalueX) {
            valueX = cvalueX;
        }
        void setValueY(int32_t cvalueY) {
            valueY = cvalueY;
        }

        bool operator<(const DataPoint& other) const {
        // Primary sorting key: name
            return name < other.name;
        }


    };

    class DataGroup {
    public:
        string name;
        int32_t rangeLow;
        int32_t rangeHigh;
        string nameX;
        string nameY;
        vector<DataPoint> dataPoints;
        std::map<string,int> dataPointsIndex;

        DataGroup(string cname, int32_t crangeLow, int32_t crangeHigh, string cnameX, string cnameY)
            : name(cname), rangeLow(crangeLow), rangeHigh(crangeHigh), nameX(cnameX), nameY(cnameY) {}

        
        // DataGroup(string cname,int32_t crangeLow,int32_t crangeHigh,int32_t cnameX,int32_t cnameY) {
        //     name = cname;
        //     rangeLow = crangeLow;
        //     rangeHigh = crangeHigh;
        //     nameX = cnameX;
        //     nameY = cnameY;
        // }

        void addDataPoint(string cname,int32_t cvalueX,int32_t cvalueY) {
            dataPoints.push_back(DataPoint(cname, cvalueX, cvalueY));
            dataPointsIndex.insert({cname,dataPoints.size()-1});
        }

        int getDataPointCount() {
            return dataPoints.size();
        }

        DataPoint getDataPoint(string dataPointName) {
            return dataPoints.at(dataPointsIndex.at(dataPointName));
        }
        DataPoint getDataPointAt(int idx) {
            return dataPoints.at(idx);
        }

        bool operator<(const DataGroup& other) const {
        // Primary sorting key: name
            return name < other.name;
        }

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



    string title;
    vector<DataGroup> dataGroups;
    std::map<string,int> dataGroupsIndex;
    uint8_t type;

    Chart(string chartTitle,uint8_t chartType) : title(chartTitle), type(chartType) {}

    // Chart(string chartTitle) {
    //     title = chartTitle;
    // }

    string getTitle() {
        return title;
    }

    void setTitle(string chartTitle ) {
        title = chartTitle;
    }

    void setChartType(uint8_t ctype) {
        type = ctype;
    }

    uint8_t getChartType() {
        return type;
    }

    string getChartTypeName() {
        switch(type) {
            case 1:
                return "Bar Chart";
            case 2:
                return "Line Chart";
            case 3:
                return "Pie Chart";
            case 4:
                return "Pie/Point Chart";
        }
        return "Undefined";
    }
    
    void addDataGroup(string name, int32_t rangeLow,int32_t rangeHigh,string nameX,string nameY) {
        dataGroups.push_back(DataGroup(name,rangeLow,rangeHigh,nameX,nameY));
        dataGroupsIndex.insert({name,dataGroups.size()-1});
    }

    void addDataToGroup(string name, string dataPointName, int32_t cvalueX,int32_t cvalueY) {
        dataGroups.at(dataGroupsIndex.at(name)).addDataPoint(dataPointName,cvalueX,cvalueY);
    }

    DataGroup getDataGroup(string dataGroupName) {
        return dataGroups.at(dataGroupsIndex.at(dataGroupName));
    }
    DataGroup getDataGroupAt(int idx) {
        return dataGroups.at(idx);
    }





#ifdef USE_ILI9341
    friend void drawBarGraph(Adafruit_ILI9341& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor);
    friend void drawLineGraph(Adafruit_ILI9341& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor);
    friend void drawPieGraph(Adafruit_ILI9341& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor);
    friend void drawPiePointGraph(Adafruit_ILI9341& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor);
#endif
#ifdef USE_ST7789
    friend void drawBarGraph(Adafruit_ST7789& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor);
    friend void drawLineGraph(Adafruit_ST7789& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor);
    friend void drawPieGraph(Adafruit_ST7789& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor);
    friend void drawPiePointGraph(Adafruit_ST7789& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor);
#endif
};


#ifdef USE_ILI9341
void drawBarGraph(Adafruit_ILI9341&, Chart&, uint16_t, uint16_t, uint16_t);
void drawLineGraph(Adafruit_ILI9341&, Chart&, uint16_t, uint16_t, uint16_t);
void drawPieGraph(Adafruit_ILI9341&, Chart&, uint16_t, uint16_t, uint16_t);
void drawPiePointGraph(Adafruit_ILI9341&, Chart&, uint16_t, uint16_t, uint16_t);
typedef void (*FunctionCall)(Adafruit_ILI9341&, Chart&, uint16_t, uint16_t, uint16_t);
#endif

#ifdef USE_ST7789
void drawBarGraph(Adafruit_ST7789&, Chart&, uint16_t, uint16_t, uint16_t);
void drawLineGraph(Adafruit_ST7789&, Chart&, uint16_t, uint16_t, uint16_t);
void drawPieGraph(Adafruit_ST7789&, Chart&, uint16_t, uint16_t, uint16_t);
void drawPiePointGraph(Adafruit_ST7789&, Chart&, uint16_t, uint16_t, uint16_t);
typedef void (*FunctionCall)(Adafruit_ST7789&, Chart&, uint16_t, uint16_t, uint16_t);
#endif

//=========================================================================================================





#ifdef USE_ILI9341
    void drawBarGraph(Adafruit_ILI9341& tftPtr, Chart& chartPtr) {
#endif
#ifdef USE_ST7789
    void drawBarGraph(Adafruit_ST7789& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor) {
#endif
    uint16_t screenWidthPx = tftPtr.width();
    uint16_t screenHeightPx = tftPtr.height();
    uint8_t screenRotation = tftPtr.getRotation();
    uint16_t xMarginPx = 8, yMarginPx = 20;
    uint16_t barWidthPx = 5;
    uint16_t xSpaceBetweenPx = ((screenWidthPx- (2*xMarginPx)) - (chartPtr.getDataGroupAt(0).getDataPointCount()*barWidthPx))/(chartPtr.getDataGroupAt(0).getDataPointCount()-1);
    uint16_t yPixelsPerUnit = (screenHeightPx-(2*yMarginPx))/(chartPtr.getDataGroupAt(0).rangeHigh - chartPtr.getDataGroupAt(0).rangeLow);
    uint16_t nextX = xMarginPx + xSpaceBetweenPx;
    
    tftPtr.setFont(&FreeSansBold9pt7b);
    tftPtr.fillScreen(bgColor);
    tftPtr.setTextColor(textColor);
    tftPtr.setCursor(3, yMarginPx/2);
    tftPtr.println(chartPtr.getTitle().data());

    tftPtr.setRotation(2);
    tftPtr.setFont(&FreeSans9pt7b);
    tftPtr.setCursor((screenHeightPx/2)-(yMarginPx*2),xMarginPx-(xMarginPx/2));
    tftPtr.print(chartPtr.getDataGroup(0).nameY.data());   

    tftPtr.setRotation(screenRotation);
    tftPtr.setFont(&FreeSans9pt7b);
    tftPtr.setCursor((screenWidthPx/2)-(xMarginPx*2),screenHeightPx-(yMarginPx/2));
    tftPtr.print(chartPtr.getDataGroup(0).nameX.data());   


    tftPtr.drawLine(xMarginPx,screenHeightPx-yMarginPx,screenWidthPx-xMarginPx,screenHeightPx-yMarginPx, fgColor);
    tftPtr.drawLine(xMarginPx,screenHeightPx-yMarginPx,xMarginPx,yMarginPx, fgColor);

    for(int i=0; i< chartPtr.getDataGroupAt(0).getDataPointCount(); i++) {
        uint16_t height = yPixelsPerUnit * chartPtr.getDataGroupAt(0).getDataPointAt(i).getValueY();
        tftPtr.fillRect(nextX,yMarginPx,barWidthPx,height, fgColor);
        nextX += xSpaceBetweenPx+barWidthPx;
    }
  

}

#ifdef USE_ILI9341
    void drawLineGraph(Adafruit_ILI9341& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor) {
#endif
#ifdef USE_ST7789
    void drawLineGraph(Adafruit_ST7789& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor) {
#endif

}

#ifdef USE_ILI9341
    void drawPieGraph(Adafruit_ILI9341& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor) {
#endif
#ifdef USE_ST7789
    void drawPieGraph(Adafruit_ST7789& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor) {
#endif

}



#ifdef USE_ILI9341
    void drawPiePointGraph(Adafruit_ILI9341& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor) {
#endif
#ifdef USE_ST7789
    void drawPiePointGraph(Adafruit_ST7789& tftPtr,  Chart& chartPtr, uint16_t bgColor, uint16_t fgColor, uint16_t textColor) {
#endif

}



Chart setChartTestData() {
    Chart testBarChart("Test Bar Chart", Chart::BAR_CHART);
    testBarChart.addDataGroup("DG1", 0, 10,"Time","Mass");
    testBarChart.addDataToGroup("DG1", "DP1", 1, 8);
    testBarChart.addDataToGroup("DG1", "DP2", 2, 6);
    testBarChart.addDataToGroup("DG1", "DP3", 3, 7);
    testBarChart.addDataToGroup("DG1", "DP4", 4, 5);
    testBarChart.addDataToGroup("DG1", "DP5", 5, 3);
    testBarChart.addDataToGroup("DG1", "DP6", 5, 0);
    testBarChart.addDataToGroup("DG1", "DP7", 5, 5);
    return testBarChart;
}