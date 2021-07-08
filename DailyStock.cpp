#include "DailyStock.h"
#include <iostream>

using namespace std;

DailyStock::DailyStock(string n, std::string d, float c, int v, float o, float h, float l) {
	this->name = n;
	this->date  = Date(d);
	this->close = c;
	this->volume = v;
	this->open = o;
	this->high = h;
	this->low = l;

	this->dailyReturn = (((c - o) / o) * 100);
}

string DailyStock::GetName() {
	return this->name;
}

Date DailyStock::GetDate() {
	return this->date;
}
float DailyStock::GetClose() {
	return this->close;
}
int DailyStock::GetVolume() {
	return this->volume;
}
float DailyStock::GetOpen() {
	return this->open;
}
float DailyStock::GetHigh() {
	return this->high;
}
float DailyStock::GetLow() {
	return this->low;
}
float DailyStock::GetDailyReturn() {
	return this->dailyReturn;
}