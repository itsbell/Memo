//Stack.h
#ifndef _STACK_H
#define _STACK_H

#include "Array.h"

class Command;
class Stack {

	public:
		Stack(Long index = 256);
		~Stack();

		void Push(Command* command);
		Command* Pop();
		Command* Top();

		Long GetCapacity() const;
		Long GetLength() const;

	private:
		Array<Command*> commands;
		Long capacity;
		Long length;
};

inline Long Stack::GetCapacity() const {
	return this->capacity;
}

inline Long Stack::GetLength() const {
	return this->length;
}

#endif //_STACK_H