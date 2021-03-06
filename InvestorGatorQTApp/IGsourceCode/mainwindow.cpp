#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <QApplication>
#include <QVector>
#include <vector>
#include <QMessageBox>

#include "Date.h"

using namespace::std;

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

void merge(vector<DailyStock>& arr,int l, int m, int r);
// void PrintArray(DailyStock *arr[], int size);

// code referenced from module 6 PPT
void mergeSort(vector<DailyStock>& arr, int l, int r){

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
void merge(vector<DailyStock>& arr, int l, int m, int r){
    // create x <- arr[1 -> m], y <- arr[m+1 -> r]
    // cout << "array: " << endl;
    // PrintArray(arr, r);
    // cout << endl;
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<DailyStock> x;
    vector<DailyStock> y;

    // DailyStock* x = arr[n1];
    // DailyStock* y = arr[n2];

    for (int i = 0; i < n1; i++)
        x.push_back(arr[l+i]);
    for (int j = 0; j < n2; j++)
        y.push_back(arr[m + 1 + j]);


    // merge the arrays x and y into arr
    int i, j, k;
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2){
        if (x[i].GetDailyReturn() <= y[j].GetDailyReturn()){
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

// ================ QuickSort =================
// Sort stocks based on their daily return using quicksort

// Partition - code referenced from module 6 PPT
int partition(vector<DailyStock>& stocks, int low, int high) {

    // The first element is the pivot
    float pivot = stocks[low].GetDailyReturn();
    int up = low, down = high;
    while (up < down) {
        for (int j = up; j < high; j++) {
            if (stocks[up].GetDailyReturn() > pivot) {
                break;
            }
            up++;
        }
        for (int j = high; j > low; j--) {
            if (stocks[down].GetDailyReturn() < pivot) {
                break;
            }
            down--;
        }
        if (up < down) {
            swap(stocks[up], stocks[down]);
        }
    }
    swap(stocks[low], stocks[down]);
    return down;
}

// Quick Sort - code referenced from module 6 PPT
void quickSort(vector<DailyStock>& stocks, int low, int high) {
    if (low < high) {
        float pivot = partition(stocks, low, high);
        quickSort(stocks, low, pivot - 1);
        quickSort(stocks, pivot + 1, high);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
// ================ Search =================

// return a vector of stocks given a time period
vector<DailyStock> Search(string symbol, Date d1, Date d2, vector<DailyStock> fullArr) {
    vector <DailyStock> retVec;
    if (symbol == "")
    {
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
                            else if (d1.GetYear() == d2.GetYear() && d1.GetMonth() != fullArr[i].GetDate().GetMonth() && d2.GetMonth() == fullArr[i].GetDate().GetMonth()) {
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
    }
    else {
        for (int i = 0; i < fullArr.size(); i++) {
            {
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
                                else if (d1.GetYear() == d2.GetYear() && d1.GetMonth() != fullArr[i].GetDate().GetMonth() && d2.GetMonth() == fullArr[i].GetDate().GetMonth()) {
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
        }
    }
    return retVec;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
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


           vector<DailyStock> stocks;
           //cout << "Total daily stocks: " << dailyStocks.size() << endl;
           for (int i = 0; i < dailyStocks.size(); i++) {
               stocks.push_back(dailyStocks.at(i));
           }

           // testing the first dailyStocks.size() stocks
               vector<DailyStock> temp;
               for (int i = 0; i < dailyStocks.size(); i++) {
                   temp.push_back(dailyStocks.at(i));
               }

           // get runtimes to display for each sort
           clock_t t;
           t = clock();
           mergeSort(stocks, 0, dailyStocks.size() - 1);
           t = clock() - t;
           float tempMST = ((float)t/CLOCKS_PER_SEC);
           ui->label_5->setNum(tempMST);

           t = clock();
           quickSort(temp, 0, temp.size() - 1);
           t = clock()-t;
           float tempQST = ((float)t/CLOCKS_PER_SEC);
           ui->label_7->setNum(tempQST);

          //convert passed dates to create date objects for search method
          string temp1 = sDate.toStdString();
          string temp2 = eDate.toStdString();
          string compSym = company.toStdString();
          Date d1 = Date(temp1);
          Date d2 = Date(temp2);

          vector<DailyStock> tempVec;
          string displayString = "";

         // clear list window on button press
         while(ui->listWidget->count()>0)
         {
            ui->listWidget->takeItem(0);
         }

        int size = 10;

        //count for list widget display
        int count = 1;

        // create vector to display from search method
        tempVec = Search(compSym, d1, d2, stocks);

        if (tempVec.size() < 10)
           {
            for (int i = tempVec.size()-1; i >= 0; i--)
            {
                 displayString = to_string(count) + ". " + tempVec[i].GetSymbol() + "   " + tempVec[i].GetName() + "   " + to_string(tempVec[i].GetDate().GetMonth()) + "/" + to_string(tempVec[i].GetDate().GetDay()) + "/" + to_string(tempVec[i].GetDate().GetYear()) + "   " + to_string(tempVec[i].GetDailyReturn()) + "%";
                 ui->listWidget->addItem(QString::fromStdString(displayString));
                 count++;
            }
           }
        else {
           for (int i = tempVec.size()-1; i >= tempVec.size()-size; i--)
           {
               displayString = to_string(count) + ". " + tempVec[i].GetSymbol() + "   " + tempVec[i].GetName() + "   " + to_string(tempVec[i].GetDate().GetMonth()) + "/" + to_string(tempVec[i].GetDate().GetDay()) + "/" + to_string(tempVec[i].GetDate().GetYear()) + "   " + to_string(tempVec[i].GetDailyReturn()) + "%";
               ui->listWidget->addItem(QString::fromStdString(displayString));
               count++;
           }
        }

}


void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{

}


void MainWindow::on_dateEdit_2_userDateChanged(const QDate &date)
{

}


void MainWindow::on_pushButton_2_clicked()
{
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


           vector<DailyStock> stocks;
           //cout << "Total daily stocks: " << dailyStocks.size() << endl;
           for (int i = 0; i < dailyStocks.size(); i++) {
               stocks.push_back(dailyStocks.at(i));
           }

           // testing the first dailyStocks.size() stocks
               vector<DailyStock> temp;
               for (int i = 0; i < dailyStocks.size(); i++) {
                   temp.push_back(dailyStocks.at(i));
               }


           clock_t t;
           t = clock();
           mergeSort(stocks, 0, dailyStocks.size() - 1);
           t = clock() - t;
           float tempMST = ((float)t/CLOCKS_PER_SEC);
           ui->label_5->setNum(tempMST);

           t = clock();
           quickSort(temp, 0, temp.size() - 1);
           t = clock()-t;
           float tempQST = ((float)t/CLOCKS_PER_SEC);
           ui->label_7->setNum(tempQST);





           string temp1 = sDate.toStdString();
           string temp2 = eDate.toStdString();
           string compSym = company.toStdString();
           Date d1 = Date(temp1);
           Date d2 = Date(temp2);

           vector<DailyStock> tempVec;
           string displayString = "";

           while(ui->listWidget->count()>0)
           {
              ui->listWidget->takeItem(0);
           }

           int count = 1;

           tempVec = Search(compSym, d1, d2, stocks);
           for (int i = tempVec.size()-1; i >= 0; i--)
           {
                displayString = to_string(count) + ". " + tempVec[i].GetSymbol() + "   " + tempVec[i].GetName() + "   " + to_string(tempVec[i].GetDate().GetMonth()) + "/" + to_string(tempVec[i].GetDate().GetDay()) + "/" + to_string(tempVec[i].GetDate().GetYear()) + "   " + to_string(tempVec[i].GetDailyReturn()) + "%";
                ui->listWidget->addItem(QString::fromStdString(displayString));
                count++;
           }

}


void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    this->company = arg1;
}



void MainWindow::on_lineEdit_2_textEdited(const QString &arg1)
{
    this->sDate = arg1;
}


void MainWindow::on_lineEdit_3_textEdited(const QString &arg1)
{
    this->eDate = arg1;
}

