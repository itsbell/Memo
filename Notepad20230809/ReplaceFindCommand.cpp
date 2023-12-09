//ReplaceFindCommand.cpp
#include "ReplaceFindCommand.h"
#include "ReplaceCurrentCommand.h"
#include "FindNextCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ReplaceFindCommand::ReplaceFindCommand(TextEditor* textEditor)
	:MacroCommand() {
	this->textEditor = textEditor;

	this->Add(new ReplaceCurrentCommand(textEditor, true, true, this));
	this->Add(new FindNextCommand(textEditor, true, true, this));
}

ReplaceFindCommand::ReplaceFindCommand(TextEditor* textEditor, bool unDoReDo)
	:MacroCommand() {
	this->textEditor = textEditor;
	this->unDoReDo = true;
}

ReplaceFindCommand::~ReplaceFindCommand() {

}

Command* ReplaceFindCommand::EmptyClone() {
	return new ReplaceFindCommand(this->textEditor, true);
}