//ReplaceInfoStack.h
#ifndef _REPLACEINFOSTACK_H
#define _REPLACEINFOSTACK_H

#include "Array.h"

class Position;

class ReplaceInfoStack {

	public:
		ReplaceInfoStack(Long capacity = 256);
		ReplaceInfoStack(const ReplaceInfoStack& source);
		~ReplaceInfoStack();

		void Push(Position* position);
		Position* Pop();
		Position* Top();

		Long GetCapacity() const;
		Long GetLength() const;

	private:
		Array<Position*> replaceInfos;
		Long capacity;
		Long length;

};

inline Long ReplaceInfoStack::GetCapacity() const {
	return this->capacity;
}

inline Long ReplaceInfoStack::GetLength() const {
	return this->length;
}

#endif //_REPLACEINFOSTACK_H