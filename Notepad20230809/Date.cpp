//Date.cpp

#include "Date.h"
#include <ctime>
#include <cstdio>
#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
함수명칭: Date
기    능: 객체를 생성하다.
입    력: 없음
출    력: 없음
*/
Date::Date() {
	this->year = 1900;
	this->month = JAN;
	this->day = 1;
	this->weekDay = MON;
}

/*
메세지명칭 : Date
기      능 : 객체를 생성한다.
입      력 : 년도, 월, 일
출      력 : 없음
*/
Date::Date(int year, Month month, int day) {
	struct tm date = { 0, };

	date.tm_year = year - 1900;
	date.tm_mon = month - 1;
	date.tm_mday = day;

	mktime(&date);

	this->year = date.tm_year + 1900;
	this->month = static_cast<Month>(date.tm_mon + 1);
	this->day = date.tm_mday;
	this->weekDay = static_cast<WeekDay>(date.tm_wday);
}

/*
메세지명칭 : Date
기      능 : 객체를 생성한다.
입      력 : 날짜
출      력 : 없음
*/
Date::Date(char(*date)) {
	struct tm date_ = { 0, };

	sscanf(date, "%4d%2d%2d", &(date_.tm_year), &(date_.tm_mon), &(date_.tm_mday));

	date_.tm_year = date_.tm_year - 1900;
	date_.tm_mon = date_.tm_mon - 1;

	mktime(&date_);

	this->year = date_.tm_year + 1900;
	this->month = static_cast<Month>(date_.tm_mon + 1);
	this->day = date_.tm_mday;
	this->weekDay = static_cast<WeekDay>(date_.tm_wday);
}

/*
메세지명칭 : Date
기      능 : 객체를 생성한다.
입      력 : 날짜
출      력 : 없음
*/
Date::Date(const Date& source) {
	this->year = source.year;
	this->month = source.month;
	this->day = source.day;
	this->weekDay = source.weekDay;
}


/*
함수명칭: Today
기    능: 오늘 날짜를 구하다.
입    력: 없음
출    력: 날짜
*/
Date Date::Today() {
	time_t timer;
	struct tm* today;
	Date today_;

	time(&timer);
	today = localtime(&timer);
	today_.year = today->tm_year + 1900;
	today_.month = static_cast<Month>(today->tm_mon + 1);
	today_.day = today->tm_mday;
	today_.weekDay = static_cast<WeekDay>(today->tm_wday);

	return today_;
}

/*
함수명칭: Tomorrow
기    능: 내일 날짜를 구하다.
입    력: 없음
출    력: 날짜
*/
Date Date::Tomorrow() {
	struct tm tomorrow_ = { 0, };
	Date tomorrow;

	tomorrow_.tm_year = this->year - 1900;
	tomorrow_.tm_mon = this->month - 1;
	tomorrow_.tm_mday = this->day + 1;

	mktime(&tomorrow_);

	tomorrow.year = tomorrow_.tm_year + 1900;
	tomorrow.month = static_cast<Month>(tomorrow_.tm_mon + 1);
	tomorrow.day = tomorrow_.tm_mday;
	tomorrow.weekDay = static_cast<WeekDay>(tomorrow_.tm_wday);

	return tomorrow;
}

/*
함수명칭: Yesterday
기    능: 어제 날짜를 구하다.
입    력: 없음
출    력: 날짜
*/
Date Date::Yesterday() {
	struct tm yesterday_ = { 0, };
	Date yesterday;

	yesterday_.tm_year = this->year - 1900;
	yesterday_.tm_mon = this->month - 1;
	yesterday_.tm_mday = this->day - 1;

	mktime(&yesterday_);

	yesterday.year = yesterday_.tm_year + 1900;
	yesterday.month = static_cast<Month>(yesterday_.tm_mon + 1);
	yesterday.day = yesterday_.tm_mday;
	yesterday.weekDay = static_cast<WeekDay>(yesterday_.tm_wday);

	return yesterday;
}

/*
함수명칭: PreviousDate
기    능: 입력받은 날수 이전 날짜를 구하다.
입    력: 날수
출    력: 날짜
*/
Date Date::PreviousDate(int days) {
	struct tm previousDate_ = { 0, };
	Date previousDate;

	previousDate_.tm_year = this->year - 1900;
	previousDate_.tm_mon = this->month - 1;
	previousDate_.tm_mday = this->day - days;

	mktime(&previousDate_);

	previousDate.year = previousDate_.tm_year + 1900;
	previousDate.month = static_cast<Month>(previousDate_.tm_mon + 1);
	previousDate.day = previousDate_.tm_mday;
	previousDate.weekDay - static_cast<WeekDay>(previousDate_.tm_wday);

	return previousDate;
}

/*
함수명칭: NextDate
기    능: 입력받은 날수 이후 날짜를 구하다.
입    력: 날수
출    력: 날짜
*/
Date Date::NextDate(int days) {
	struct tm nextDate_ = { 0, };
	Date nextDate;

	nextDate_.tm_year = this->year - 1900;
	nextDate_.tm_mon = this->month - 1;
	nextDate_.tm_mday = this->day + days;

	mktime(&nextDate_);

	nextDate.year = nextDate_.tm_year + 1900;
	nextDate.month = static_cast<Month>(nextDate_.tm_mon + 1);
	nextDate.day = nextDate_.tm_mday;
	nextDate.weekDay = static_cast<WeekDay>(nextDate_.tm_wday);

	return nextDate;
}

/*
함수명칭: IsLessThan
기    능: 두 날짜를 비교하여 더 작은지 확인하다.
입    력: 날짜
출    력: Boolean
*/
bool Date::IsLessThan(const Date& other) {
	bool ret = false;

	if (this->year < other.year) {
		ret = true;
	}
	else if (this->year == other.year && this->month < other.month) {
		ret = true;
	}
	else if (this->year == other.year && this->month == other.month && this->day < other.day) {
		ret = true;
	}

	return ret;
}

/*
함수명칭: IsGreaterThan
기    능: 두 날짜를 비교하여 더 큰지 확인하다.
입    력: 날짜
출    력: Boolean
*/
bool Date::IsGreaterThan(const Date& other) {
	bool ret = false;

	if (this->year > other.year) {
		ret = true;
	}

	else if (this->year == other.year && this->month > other.month) {
		ret = true;
	}

	else if (this->year == other.year && this->month == other.month && this->day > other.day) {
		ret = true;
	}

	return ret;
}

/*
함수명칭: IsEqual
기    능: 두 날짜가 같은지 확인하다.
입    력: 날짜
출    력: Boolean
*/
bool Date::IsEqual(const Date& other) {
	bool ret = false;
	
	if (this->year == other.year && this->month == other.month && this->day == other.day) {
		ret = true;
	}

	return ret;
}

/*
함수명칭: IsNotEqual
기    능: 두 날짜가 같지 않은지 확인하다.
입    력: 날짜
출    력: Boolean
*/
bool Date::IsNotEqual(const Date& other) {
	bool ret = false;

	if (this->year != other.year || this->month != other.month || this->day != other.day) {
		ret = true;
	}

	return ret;
}

/*
함수명칭: operator char*
기    능: 형변환
입    력: 없음
출    력: 없음
*/
Date::operator char* () {
	static char buff[11];

	sprintf(buff, "%4d-%02d-%02d", this->year, this->month, this->day);

	return buff;
}

/*
함수명칭: ~Date
기    능: 객체를 소멸시키다.
입    력: 없음
출    력: 없음
*/
Date::~Date(){

}

/*
함수명칭 : operator--
기    능 : 날짜 데이터에 대해 --(전위) 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/
Date& Date::operator--() {
	struct tm yesterday_ = { 0, };

	yesterday_.tm_year = this->year - 1900;
	yesterday_.tm_mon = this->month - 1;
	yesterday_.tm_mday = this->day - 1;

	mktime(&yesterday_);

	this->year = yesterday_.tm_year + 1900;
	this->month = static_cast<Month>(yesterday_.tm_mon + 1);
	this->day = yesterday_.tm_mday;
	this->weekDay = static_cast<WeekDay>(yesterday_.tm_wday);

	return *this;
}

/*
함수명칭 : operator--
기    능 : 날짜 데이터에 대해 --(후위) 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/
Date Date::operator--(int) {
	struct tm yesterday_ = { 0, };
	Date yesterday(*this);

	yesterday_.tm_year = this->year - 1900;
	yesterday_.tm_mon = this->month - 1;
	yesterday_.tm_mday = this->day - 1;

	mktime(&yesterday_);

	this->year = yesterday_.tm_year + 1900;
	this->month = static_cast<Month>(yesterday_.tm_mon + 1);
	this->day = yesterday_.tm_mday;
	this->weekDay = static_cast<WeekDay>(yesterday_.tm_wday);

	return yesterday;
}


/*
함수명칭 : operator++
기    능 :  날짜 데이터에 대해 ++(전위) 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/


Date& Date::operator++() {
	struct tm tomorrow_ = { 0, };

	tomorrow_.tm_year = this->year - 1900;
	tomorrow_.tm_mon = this->month - 1;
	tomorrow_.tm_mday = this->day + 1;

	mktime(&tomorrow_);

	this->year = tomorrow_.tm_year + 1900;
	this->month = static_cast<Month>(tomorrow_.tm_mon + 1);
	this->day = tomorrow_.tm_mday;
	this->weekDay = static_cast<WeekDay>(tomorrow_.tm_wday);

	return *this;
}


/*
함수명칭 : operator++
기    능 : 날짜 데이터에 대해 ++(후위) 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/
Date Date::operator++(int) {
	struct tm tomorrow_ = { 0, };
	Date tomorrow(*this);

	tomorrow_.tm_year = this->year - 1900;
	tomorrow_.tm_mon = this->month - 1;
	tomorrow_.tm_mday = this->day + 1;

	mktime(&tomorrow_);

	this->year = tomorrow_.tm_year + 1900;
	this->month = static_cast<Month>(tomorrow_.tm_mon + 1);
	this->day = tomorrow_.tm_mday;
	this->weekDay = static_cast<WeekDay>(tomorrow_.tm_wday);

	return tomorrow;
}


/*
함수명칭 : operator-
기    능 : 날짜 데이터에 대해 - 연산이 가능하도록 해주다.
입    력 : 날수
출    력 : 없음
*/
Date Date::operator-(int days) {
	struct tm previousDate_ = { 0, };
	Date previousDate;

	previousDate_.tm_year = this->year - 1900;
	previousDate_.tm_mon = this->month - 1;
	previousDate_.tm_mday = this->day - days;

	mktime(&previousDate_);

	previousDate.year = previousDate_.tm_year+1900;
	previousDate.month = static_cast<Month>(previousDate_.tm_mon + 1);
	previousDate.day = previousDate_.tm_mday;
	previousDate.weekDay = static_cast<WeekDay>(previousDate_.tm_wday);

	return previousDate;
}


/*
함수명칭 : operator+
기    능 : 날짜 데이터에 대해 + 연산이 가능하도록 해주다.
입    력 : 날수
출    력 : 없음
*/


Date Date::operator+(int days) {
	struct tm nextDate_ = { 0, };
	Date nextDate;

	nextDate_.tm_year = this->year - 1900;
	nextDate_.tm_mon = this->month - 1;
	nextDate_.tm_mday = this->day + days;

	mktime(&nextDate_);

	nextDate.year = nextDate_.tm_year + 1900;
	nextDate.month = static_cast<Month>(nextDate_.tm_mon + 1);
	nextDate.day = nextDate_.tm_mday;
	nextDate.weekDay = static_cast<WeekDay>(nextDate_.tm_wday);

	return nextDate;

}

/*
함수명칭 : operator<
기    능 : 날짜 데이터에 대해 < 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/
bool Date::operator<(const Date& other) {
	bool ret = false;

	if (this->year < other.year) {
		ret = true;
	}

	else if (this->year == other.year && this->month < other.month) {
		ret = true;
	}

	else if (this->year == other.year && this->month == other.month && this->day < other.day) {
		ret = true;
	}

	return ret;
}


/*
함수명칭 : operator<=
기    능 : 날짜 데이터에 대해 <= 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/
bool Date::operator<=(const Date& other) {
	bool ret = false;

	if (this->year < other.year) {
		ret = true;
	}

	else if (this->year == other.year && this->month < other.month) {
		ret = true;
	}

	else if (this->year == other.year && this->month == other.month && this->day <= other.day) {
		ret = true;
	}

	return ret;
}


/*
함수명칭 : operator>
기    능 : 날짜 데이터에 대해 > 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/
bool Date::operator>(const Date& other) {
	bool ret = false;

	if (this->year > other.year) {
		ret = true;
	}

	else if (this->year == other.year && this->month > other.month) {
		ret = true;
	}

	else if (this->year == other.year && this->month == other.month && this->day > other.day) {
		ret = true;
	}
	return ret;
}




/*
함수명칭 : operator>=
기    능 : 날짜 데이터에 대해 >= 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/
bool Date::operator>=(const Date& other) {
	bool ret = false;

	if (this->year > other.year) {
		ret = true;
	}

	else if (this->year == other.year && this->month > other.month) {
		ret = true;
	}

	else if (this->year == other.year && this->month == other.month && this->day >= other.day) {
		ret = true;
	}

	return ret;
}



/*
함수명칭 : operator ==
기    능 : 날짜 데이터에 대해 == 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/
bool Date::operator == (const Date& other) {
	bool ret = false;

	if (this->year == other.year && this->month == other.month && this->day == other.day) {
		ret = true;
	}

	return ret;
}

/*
함수명칭 : operator !=
기    능 : 날짜 데이터에 대해 != 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/
bool Date::operator != (const Date& other) {
	bool ret = false;

	if (this->year != other.year || this->month != other.month || this->day != other.day) {
		ret = true;
	}
	return ret;
}


/*
함수명칭 : operator =
기    능 : 날짜 데이터에 대해 = 연산이 가능하도록 해주다.
입    력 : 없음
출    력 : 없음
*/
Date& Date::operator = (const Date& source) {
	this->year = source.year;
	this->month = source.month;
	this->day = source.day;
	this->weekDay = source.weekDay;

	return *this;
}

#if 0
#include <iostream> //C++ Class Libray -> Standard Template class Library (STL)
using namespace std;

int main(int argc, char* argv[]) {
	Date date;
	Date today;
	Date tomorrow;
	Date yesterday;
	Date previousDate3Days;
	Date nextDate;
	Date previousDate1Days;
	Date previousDate;
	bool ret;
	int i;

	today = Date::Today(); //function call
	cout << today.GetYear() << "-" << today.GetMonth() << "-" << today.GetDay() << endl;

	tomorrow = today.Tomorrow();
	cout << tomorrow.GetYear() << "-" << tomorrow.GetMonth() << "-" << tomorrow.GetDay() << endl;

	yesterday = today.Yesterday();
	cout << yesterday.GetYear() << "-" << yesterday.GetMonth() << "-" << yesterday.GetDay() << endl;
	
	previousDate3Days = today.PreviousDate(3);
	cout << previousDate3Days.GetYear() << "-" << previousDate3Days.GetMonth() << "-" << previousDate3Days.GetDay() << endl;

	ret = previousDate3Days.IsLessThan(today);
	if (ret == true) {
		cout << "참 입니다." << endl;
	}

	nextDate = today.NextDate(100);
	cout << nextDate.GetYear() << "-" << nextDate.GetMonth() << "-" << nextDate.GetDay() << endl;

	ret = nextDate.IsGreaterThan(yesterday);
	if (ret == true) {
		cout << "참 입니다." << endl;
	}

	ret = nextDate.IsNotEqual(yesterday);
	if (ret == true) {
		cout << "같은날이 아닙니다" << endl;
	}

	previousDate1Days = today.PreviousDate(1);
	cout << previousDate1Days.GetYear() << "-" << previousDate1Days.GetMonth() << "-" << previousDate1Days.GetDay() << endl;

	ret = previousDate1Days.IsEqual(yesterday);
	if (ret == true) {
		cout << "같은날 입니다." << endl;
	}

	Date birthday(2022, OCT, 29);
	cout << birthday.GetYear() << "-"<< birthday.GetMonth() << "-" << birthday.GetDay() << endl;

	Date another((char*)"20221029");
	cout << another.GetYear() << "-" << another.GetMonth() << "-" << another.GetDay() << endl;

	Date other(today);
	cout << static_cast<char*>(other) << endl;

	yesterday = --today;
	cout << yesterday.GetYear() << "-" << yesterday.GetMonth() << "-" << yesterday.GetDay() << endl;

	++today;
	cout << today.GetYear() << "-" << today.GetMonth() << "-" << today.GetDay() << endl;

	for (i = 1; i <= 5; i++) {
		cout << today.GetYear() << "-" << today.GetMonth() << "-" << today.GetDay() << endl;
		today--;
	}

	for (i = 1; i <= 5; i++) {
		cout << today.GetYear() << "-" << today.GetMonth() << "-" << today.GetDay() << endl;
		today++;
	}

	previousDate = today - 1;
	cout << previousDate.GetYear() << "-" << previousDate.GetMonth() << "-" << previousDate.GetDay() << endl;

	nextDate = today + 5;
	cout << nextDate.GetYear() << "-" << nextDate.GetMonth() << "-" << nextDate.GetDay() << endl;

	if (today < tomorrow) {
		cout << "참 입니다" << endl;
	}

	if (today <= today) {
		cout << "참 입니다" << endl;
	}

	if (nextDate > today) {
		cout << "참 입니다" << endl;
	}

	if (nextDate >= nextDate) {
		cout << "참 입니다" << endl;
	}

	if (yesterday == previousDate) {
		cout << "같은 날 입니다" << endl;
	}

	if (today != nextDate) {
		cout << "다른 날 입니다" << endl;
	}

	return 0;
}
#endif