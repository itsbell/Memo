//Stack.cpp
#include "Stack.h"
#include "UnDoReDoCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Stack::Stack(Long capacity) 
	: commands(capacity){
	
	this->capacity = capacity;
	this->length = 0;
}

Stack::~Stack() {
	Long i = 0;

	while (i < this->length) {
		if (this->commands[i] != 0) {
			delete this->commands[i];
		}
		i++;
	}
}

void Stack::Push(Command* command) {

	if (this->length < this->capacity) {
		this->commands.Store(this->length, command);
	}

	else {
		this->commands.AppendFromRear(command);
		this->capacity++;
	}

	this->length++;
}

Command* Stack::Pop() {
	Command* command = 0;

	if (this->length > 0) {
		command = this->commands.GetAt(this->length - 1);
		this->commands.DeleteFromRear();
		this->capacity--;
		this->length--;
	}
	
	return command;
}

Command* Stack::Top() {
	Command* command = 0;

	if (this->length > 0) {
		command = this->commands.GetAt(this->length - 1);
	}
	
	return command;
}