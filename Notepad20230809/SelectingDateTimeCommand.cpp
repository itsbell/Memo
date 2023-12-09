//SelectingDateTimeCommand.cpp
#include "SelectingDateTimeCommand.h"
#include "DeleteCommand.h"
#include "DateTimeCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SelectingDateTimeCommand::SelectingDateTimeCommand(TextEditor* textEditor)
	:MacroCommand() {
	this->textEditor = textEditor;
	this->unDoReDo = true;
	this->Add(new DeleteCommand(textEditor, true));
	this->Add(new DateTimeCommand(textEditor, true, true));
}

SelectingDateTimeCommand::SelectingDateTimeCommand(TextEditor* textEditor, bool unDoReDo)
	:MacroCommand() {
	this->textEditor = textEditor;
	this->unDoReDo = true;
}

SelectingDateTimeCommand::~SelectingDateTimeCommand() {

}

Command* SelectingDateTimeCommand::EmptyClone() {
	return new SelectingDateTimeCommand(this->textEditor, true);
}