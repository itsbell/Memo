//UnDoCommand.cpp
#include "UnDoCommand.h"
#include "TextEditor.h"
#include "MacroCommand.h"
#include "Stack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UnDoCommand::UnDoCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

UnDoCommand::~UnDoCommand() {

}

void UnDoCommand::Execute() {
	Command* command = 0;
	
	if (this->textEditor->unDoCommands != 0) {
		command = this->textEditor->unDoCommands->Pop();
	}
	
	if (command != 0) {
		command->Unexecute();
		if (dynamic_cast<MacroCommand*>(command)) {
			delete command;
		}
	}
}

void UnDoCommand::Unexecute() {

}