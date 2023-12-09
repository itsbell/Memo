//Position.h
#ifndef _POSITION_H
#define _POSITION_H

typedef signed long int Long;

class Position {
	
	public:
		Long row;
		Long column;

	public:
		Position();
		Position(Long row, Long column);
		Position(const Position& source);
		~Position();

		void SetPosition(Long row, Long column);
		bool operator < (const Position& other);
		bool operator <= (const Position& other);
		bool operator > (const Position& other);
		bool operator >= (const Position& other);
		bool operator == (const Position& other);
		bool operator != (const Position& other);
		Position& operator = (const Position& source);

};

#endif //_POSITION_H