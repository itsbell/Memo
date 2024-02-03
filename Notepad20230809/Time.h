//Time.h
#ifndef _TIME_H
#define _TIME_H

class Time {

	public:
		Time();
		Time(const Time& source);
		~Time();

		static Time GetCurrent();
		operator char* ();
		Time operator -(const Time& source);

		int GetHour() const;
		int GetMin() const;
		int GetSec() const;
		int GetMillisec() const;

	private:
		int hour;
		int min;
		int sec;
		int millisec;
};

inline int Time::GetHour() const {
	return this->hour;
}

inline int Time::GetMin() const {
	return this->min;
}

inline int Time::GetSec() const {
	return this->sec;
}

inline int Time::GetMillisec() const {
	return this->millisec;
}


#endif //_TIME_H