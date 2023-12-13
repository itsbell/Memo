//Time.cpp
#include "Time.h"
#include <atltime.h>

#pragma warning (disable:4996)

Time::Time() {
	this->hour = 0;
	this->min = 0;
	this->sec = 0;
	this->millisec = 0;
}

Time::~Time() {

}

Time Time::GetCurrent() {
	Time current;
	SYSTEMTIME  currentTime;
	::GetLocalTime(&currentTime);
	
	current.hour = currentTime.wHour;
	current.min = currentTime.wMinute;
	current.sec = currentTime.wSecond;
	current.millisec = currentTime.wMilliseconds;
	
	return current;
}


Time::operator char* () {
	static char buff[6];

	sprintf(buff, "%02d:%02d", this->hour, this->min);

	return buff;
}