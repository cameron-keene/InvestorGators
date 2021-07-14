#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "DailyStock.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

// Read csv file containing symbol, officical name, and file name
void readNames(vector<vector<string> >& stockNames, const string file){
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

// mergeSort --- need to modify to sort based on dailyreturn
// need to modify it to accept an DailyStock arr of pointers
// need to return a arr of DailyStock Pointers
// sort based on GetDailyReturn()
void merge(DailyStock *arr[], int l, int m, int r);
// void PrintArray(DailyStock *arr[], int size);

// code referenced from module 6 PPT
void mergeSort(DailyStock *arr[], int l, int r){
    
    if(l < r){
        // m is in the point where the array is divided into two sub-arrays
        int m = l + (r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        // merge the two sorted sub-arrays
        merge(arr, l, m, r);
    }
}

// code referenced from module 6 PPT
void merge(DailyStock *arr[], int l, int m, int r){
    // create x <- arr[1 -> m], y <- arr[m+1 -> r]
    // cout << "array: " << endl;
    // PrintArray(arr, r);
    // cout << endl;
    int n1 = m - l + 1;
    int n2 = r - m;
    DailyStock* x[n1];
    DailyStock* y[n2];
    // DailyStock* x = arr[n1];
    // DailyStock* y = arr[n2];

    for (int i = 0; i < n1; i++)
        x[i] = arr[l+i];
    for (int j = 0; j < n2; j++)
        y[j] = arr[m + 1 + j];
    
    // merge the arrays x and y into arr
    int i, j, k;
    i = 0; 
    j = 0; 
    k = l;
    while (i < n1 && j < n2){
        if (x[i]->GetDailyReturn() <= y[j]->GetDailyReturn()){
            arr[k] = x[i];
            i++;
        }else{
            arr[k] = y[j];
            j++;
        }
        k++;
    }

    // when we run out of elements in either x or y append the remaining elements
    while( i < n1 ){
        arr[k] = x[i];
        i++;
        k++;
    }
    while( k < n2 ){
        arr[k] = y[j];
        j++;
        k++;
    }
}



int main(){
    
    // Storage of daily stocks from s&p 100 for 5 years
    vector<DailyStock> dailyStocks;
    
    // Read the file with the names and symbols
    vector<vector<string> > stockNames;
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

    // test case of 100 randomly picked stocks
    // stocks arr to hold 100 random DailyStock objects
    DailyStock* stocks[dailyStocks.size()];
    cout << "Total daily stocks: " << dailyStocks.size() << endl;
    for(int i = 0; i < dailyStocks.size(); i++){
        //cout << dailyStocks.at(i).GetName() << " " << dailyStocks.at(i).GetClose() << " " << dailyStocks.at(i).GetDailyReturn() << "%" << endl;
        stocks[i] = &dailyStocks.at(i);
    }

    mergeSort(stocks, 0, dailyStocks.size()-1);

    for(int i = dailyStocks.size()-1; i >= 0; i--){
        cout.precision(4);
        cout << stocks[i]->GetName() << " " << stocks[i]->GetOpen() << " " << stocks[i]->GetClose() << " " << stocks[i]->GetDailyReturn() << "%" << endl;
        //stocks[i] = &dailyStocks.at((rand() % 115000));
        
    }
    return 0;
}
