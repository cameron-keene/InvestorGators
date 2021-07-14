#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "DailyStock.h"
using namespace std;

// Read csv file containing symbol, officical name, and file name
void readNames(vector<vector<string>>& stockNames, const string file){
    ifstream names(file);
    if(names.is_open()){

        // Read heading
        string stock;
        getline(names,stock);

        // Get symbol, official name and file name
        vector<string> stockInfo;
        while(getline(names,stock)){
            string symbol, name, fileName;

            // Stream of data coming from each line of the original file
            istringstream stockName(stock); 

            getline(stockName, symbol, ',');
            getline(stockName, name, ',');
            getline(stockName, fileName, ',');

            stockInfo.push_back(symbol);
            stockInfo.push_back(name);
            stockInfo.push_back(fileName);

            stockNames.push_back(stockInfo);
            stockInfo.clear();
        }
    }
}

// Read csv file containing daily stock information (date, close, volume, open, high, low)
DailyStock readStockData(string lineFile, string symbol, string name) {

    // stream of data coming from each line of the original file
    istringstream individualInfo(lineFile);

    string date, tempClose, tempOpen, tempVolume, tempHigh, tempLow;
    float close, open, high, low;
    int volume;

    getline(individualInfo, date, ',');
    getline(individualInfo, tempClose, ',');
    getline(individualInfo, tempVolume, ',');
    getline(individualInfo, tempOpen, ',');
    getline(individualInfo, tempHigh, ',');
    getline(individualInfo, tempLow, ',');

    // Clean values, remove "$"
    tempClose.erase(tempClose.begin());
    tempOpen.erase(tempOpen.begin());
    tempHigh.erase(tempHigh.begin());
    tempLow.erase(tempLow.begin());

    close = stof(tempClose);
    open = stof(tempOpen);

    // Check when volume is N/A
    try{
    volume = stol(tempVolume);
    }catch(exception e){
        volume = 0;
    }
    high = stof(tempHigh);
    low = stof(tempLow);

    // Initialize stock obejct and return it for storage
    return DailyStock(name, symbol, date, close, volume, open, high, low);
}


vector<DailyStock> Search(Date d1, Date d2, vector<DailyStock> fullArr) {
    vector <DailyStock> retVec;
    for (int i = 0; i < fullArr.size(); i++) {
        // checks if in a valid year
        if (d1.GetYear() <= fullArr[i].GetDate().GetYear() && d2.GetYear() >= fullArr[i].GetDate().GetYear()) {
            // checks if in one of the edge years
            if (d1.GetYear() == fullArr[i].GetDate().GetYear() || d2.GetYear() == fullArr[i].GetDate().GetYear()) {
                //checks if d1 year is same as daily stock
                if (d1.GetYear() == fullArr[i].GetDate().GetYear()) {
                    //checks if d1 month is less than ds, (and if d2 month is greater than ds or if the end year is different) then adds ds
                    if (d1.GetMonth() < fullArr[i].GetDate().GetMonth() && (d2.GetMonth() > fullArr[i].GetDate().GetMonth() || d2.GetYear() != fullArr[i].GetDate().GetYear())) {
                        retVec.push_back(fullArr[i]);
                    }
                    else {
                        //checks if d1 month equals ds month and d2 month does not
                        if (d1.GetMonth() == fullArr[i].GetDate().GetMonth() && d2.GetMonth() != fullArr[i].GetDate().GetMonth()) {
                            // if d1 day is less than ds day adds ds
                            if (d1.GetDay() <= fullArr[i].GetDate().GetDay()) {
                                retVec.push_back(fullArr[i]);
                            }
                        }
                        // checks if d2 month equals ds month and d1 month does not
                        else if (d1.GetMonth() != fullArr[i].GetDate().GetMonth() && d2.GetMonth() == fullArr[i].GetDate().GetMonth() && d1.GetYear() != fullArr[i].GetDate().GetYear()) {
                            // if d2 day is greater than ds day adds ds
                            if (d2.GetDay() >= fullArr[i].GetDate().GetDay()) {
                                retVec.push_back(fullArr[i]);
                            }
                        }
                        else {
                            // if d1 month is less than or equal to ds month and d2 month is greater than or equal to ds month
                            if (d1.GetMonth() <= fullArr[i].GetDate().GetMonth() && d2.GetMonth() >= fullArr[i].GetDate().GetMonth()) {
                                // if d1 day is less than ds day and d2 day is greater than ds day adds ds
                                if (d1.GetDay() <= fullArr[i].GetDate().GetDay() && d2.GetDay() >= fullArr[i].GetDate().GetDay()) {
                                    retVec.push_back(fullArr[i]);
                                }
                            }
                        }
                    }
                }
                else {
                    //this executing means ds year equals d2 year but not d1 year
                    // checks if d2 month is greater than ds month, if so adds ds
                    if (d2.GetMonth() > fullArr[i].GetDate().GetMonth()) {
                        retVec.push_back(fullArr[i]);
                    }
                    else {
                        //checks if ds month equals d2 month 
                        if (d2.GetMonth() == fullArr[i].GetDate().GetMonth()) {
                            // checks if d2 day is greater than or equal to ds day if so adds ds
                            if (d2.GetDay() >= fullArr[i].GetDate().GetDay()) {
                                retVec.push_back(fullArr[i]);
                            }
                        }
                    }
                }
            }
            //else adds if in a middle year
            else {
                retVec.push_back(fullArr[i]);
            }
        }
    }
    return retVec;
}


int main(){
    
    // Storage of daily stocks from s&p 100 for 5 years
    vector<DailyStock> dailyStocks;
    
    // Read the file with the names and symbols
    vector<vector<string>> stockNames;
    const string NAMES = "Data/stocks.csv";
    readNames(stockNames,NAMES);
    
    // Get 5 year data from individual stocks
    string fileName, symbol, name;
    for (int i = 0; i < stockNames.size(); i++) {
        fileName = "Data/" + stockNames[i][2] + "_Data.csv";
        symbol = stockNames[i][0];
        name = stockNames[i][1];
        ifstream file(fileName);
        if (file.is_open()) {
            
            // Read heading
            string lineFile;
            getline(file, lineFile);

            // Get data line by line (by dates)
            while (getline(file, lineFile)) {
                dailyStocks.push_back(readStockData(lineFile, symbol, name));
            }
        }
    }





    //Fake date inputs, testing search function
    Date d1h = Date("12/20/2018");
    Date d2h = Date("12/25/2018");

    cout << d1h.GetMonth();
    cout << d2h.GetDay();

    vector<DailyStock> subVec = Search(d1h, d2h, dailyStocks);

    cout << endl;
    cout << endl;
    cout << endl;

    cout << subVec.size();

    cout << endl;
    cout << endl;
    cout << endl;

    //Outputing results of search 
    for (int i = 0; i < subVec.size(); i++) {
        cout << subVec[i].GetName() << "'s daily return's: " << subVec[i].GetDailyReturn() << "on: " << subVec[i].GetDate().GetMonth() << "/" << subVec[i].GetDate().GetDay() << "/" << subVec[i].GetDate().GetYear() << endl;
    }


    cout << "Total daily stocks: " << dailyStocks.size() << endl;

    return 0;
}
