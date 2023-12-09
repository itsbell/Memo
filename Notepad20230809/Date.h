//Date.h
#ifndef _DATE_H
#define _DATE_H

enum Month { JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AGU = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12 };
enum WeekDay{SUN = 0, MON = 1, TUE = 2, WED = 3, THU = 4, FRI = 5, SAT = 6};

class Date {
	public:
		Date();
		Date(const Date& source); // 복사생성자(Copy Constructor)
		Date(int year, Month month, int day); 
		Date(char(*date)); // 매겨변수가 1개 이면 형변환 관련 생성자
		~Date();
		static Date Today();
		Date Tomorrow();
		Date Yesterday();
		Date PreviousDate(int days);
		Date NextDate(int days);
		bool IsLessThan(const Date& other);
		bool IsGreaterThan(const Date& other);
		bool IsEqual(const Date& other);
		bool IsNotEqual(const Date& other);
		int GetYear() const;
		Month GetMonth() const;
		int GetDay() const;
		WeekDay GetWeekDay() const;
		operator char* (); //형변환 함수 Date->char*
		Date& operator--(); //전위
		Date operator--(int); //후위
		Date& operator++();
		Date operator++(int);
		Date operator-(int days);
		Date operator+(int days);
		bool operator<(const Date& other);
		bool operator<=(const Date& other);
		bool operator>(const Date& other);
		bool operator>=(const Date& other);
		bool operator == (const Date& other);
		bool operator != (const Date& other);
		Date& operator = (const Date& source);
	
	private:
		int year;
		Month month;
		int day;
		WeekDay weekDay;
};

inline int Date::GetYear() const {
	return this->year;
}

inline Month Date::GetMonth() const {
	return this->month;
}

inline int Date::GetDay() const {
	return this->day;
}

inline WeekDay Date::GetWeekDay() const {
	return this->weekDay;
}

#endif //_DATE_H