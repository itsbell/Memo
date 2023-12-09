//ReplaceInfoStack.cpp
#include "ReplaceInfoStack.h"
#include "Position.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <new>
#include <memory>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

ReplaceInfoStack::ReplaceInfoStack(Long capacity)
	:replaceInfos(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

ReplaceInfoStack::ReplaceInfoStack(const ReplaceInfoStack& source)
	: replaceInfos(source.replaceInfos) {
	Long i = 0;
	Position* position;
	Position* clone;

	while (i < source.length) {
		position = const_cast<ReplaceInfoStack&>(source).replaceInfos.GetAt(i);
		clone = new Position(position->row, position->column);
		this->replaceInfos.Modify(i, clone);
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

ReplaceInfoStack::~ReplaceInfoStack() {
	Long i = 0;

	while (i < this->length) {
		if (this->replaceInfos[i] != 0) {
			delete this->replaceInfos[i];
		}
		i++;
	}
}

void ReplaceInfoStack::Push(Position* position) {

	if (this->length < this->capacity) {
		this->replaceInfos.Store(this->length, position);
	}

	else {
		this->replaceInfos.AppendFromRear(position);
		this->capacity++;
	}

	this->length++;
}

Position* ReplaceInfoStack::Pop() {
	Position* position = 0;

	if (this->length > 0) {
		position = this->replaceInfos.GetAt(this->length - 1);
		this->replaceInfos.DeleteFromRear();
		this->capacity--;
		this->length--;
	}

	return position;
}

Position* ReplaceInfoStack::Top() {
	Position* position = 0;

	if (this->length > 0) {
		position = this->replaceInfos.GetAt(this->length - 1);
	}

	return position;
}
