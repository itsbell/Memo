//MacroCommand.cpp
#include "MacroCommand.h"
#include "TextEditor.h"
#include "Stack.h"
#include "UnDoReDoCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

MacroCommand::MacroCommand(Long capacity)
	:commands(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

MacroCommand::~MacroCommand() {
	Long i = 0;

	while (i < this->length) {
		if (this->commands[i] != 0) {
			delete this->commands[i];
		}
		i++;
	}
}

Long MacroCommand::Add(Command* command) {
	Long index;

	if (this->length < this->capacity) {
		index = this->commands.Store(this->length, command);
	}

	else {
		index = this->commands.AppendFromRear(command);
		this->capacity++;
	}

	this->length++;
	
	return index;
}

Long MacroCommand::Remove(Long index) {

	index = this->commands.Delete(index);

	this->capacity--;
	this->length--;

	return index;
}

void MacroCommand::Execute() {
	Long i = 0;
	Command* command;
	
	while (i < this->length) {
		command = this->commands.GetAt(i);
		command->Execute();
		i++;
	}
	if (this->unDoReDo == true) {
		Command* unDoReDoCommand = this->EmptyClone();
		i = 0;
		while (i < this->length) {
			command = this->textEditor->unDoCommands->Pop();
			dynamic_cast<UnDoReDoCommand*>(command)->pCommand = unDoReDoCommand;
			dynamic_cast<MacroCommand*>(unDoReDoCommand)->Add(command);
			i++;
		}
		this->textEditor->unDoCommands->Push(unDoReDoCommand);
	}
}

void MacroCommand::Unexecute() {
	Long i = 0;
	Command* command;
	Command* unDoReDoCommand;

	while (i < this->length) {
		command = this->commands.GetAt(i);
		command->Unexecute();
		i++;
	}
	unDoReDoCommand = this->EmptyClone();
	i = 0;
	while (i < this->length) {
		command = this->textEditor->reDoCommands->Pop();
		dynamic_cast<UnDoReDoCommand*>(command)->pCommand = unDoReDoCommand;
		dynamic_cast<MacroCommand*>(unDoReDoCommand)->Add(command);
		i++;
	}
	this->textEditor->reDoCommands->Push(unDoReDoCommand);
}
