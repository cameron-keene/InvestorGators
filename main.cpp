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

    cout << "Total daily stocks: " << dailyStocks.size() << endl;

    return 0;
}
