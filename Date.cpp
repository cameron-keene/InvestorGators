#include "Date.h"
#include <string>
#include <sstream>

Date::Date() {
	this->month = 0;
	this->day = 0;
	this->year = 0;
}

Date::Date(std::string date) {
	std::string strMonth, strDay, strYear;
	std::istringstream sstring(date);

	std::getline(sstring, strMonth, '/');
	std::getline(sstring, strDay, '/');
	std::getline(sstring, strYear);

	this->month = (short) std::stoi(strMonth);
	this->day = (short) std::stoi(strDay);
	this->year = (short) std::stoi(strYear);
}
short Date::GetMonth() {
	return month;
}
short Date::GetDay() {
	return day;
}
short Date::GetYear() {
	return year;
}