//Subject.h
#ifndef _SUBJECT_H
#define _SUBJECT_H

#include "Array.h"

class Observer;

class Subject {

	public:
		Subject(Long capacity = 256);
		~Subject();

		Long Attach(Observer* observer);
		Long Detach(Observer* observer);
		void Notify();

	private:
		Array<Observer*> observers;
		Long capacity;
		Long length;
};

int CompareObservers(void* one, void* other);

#endif //_SUBJECT_H
