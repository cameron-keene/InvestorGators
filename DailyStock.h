#include <iostream>
#include "Date.h"

class DailyStock
{
    std::string name;
    Date date;
    float close;
    int volume;
    float open;
    float high;
    float low;
    float dailyReturn;


public:
    DailyStock(std::string n, std::string d, float c, int v, float o, float h, float l);
    std::string GetName() {};
    Date GetDate() {};
    float GetClose() {};
    int GetVolume() {};
    float GetOpen() {};
    float GetHigh() {};
    float GetLow() {};
    float GetDailyReturn();
};
