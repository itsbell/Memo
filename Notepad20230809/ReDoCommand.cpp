//ReDoCommand.cpp
#include "ReDoCommand.h"
#include "TextEditor.h"
#include "Stack.h"
#include "MacroCommand.h"
#include "OnImeCharCommand.h"
#include "OnImeCompositionSelectingCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ReDoCommand::ReDoCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ReDoCommand::~ReDoCommand() {

}

void ReDoCommand::Execute() {
	Command* command = 0;
	Command* nextCommand = 0;
	if (this->textEditor->reDoCommands != 0) {
		command = this->textEditor->reDoCommands->Pop();
	}
	
	if (command != 0) {
		command->Execute();

		if (dynamic_cast<OnImeCompositionSelectingCommand*>(command)) {
			nextCommand = this->textEditor->reDoCommands->Top();
			if (dynamic_cast<OnImeCharCommand*>(nextCommand)) {
				this->textEditor->reDoCommands->Pop()->Execute();
			}
		}

		if (dynamic_cast<MacroCommand*>(command)) {
			delete command;
		}
	}
}

void ReDoCommand::Unexecute() {

}