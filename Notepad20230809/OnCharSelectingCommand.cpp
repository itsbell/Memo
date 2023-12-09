//OnCharSelectingCommand.cpp
#include "OnCharSelectingCommand.h"
#include "TextEditor.h"
#include "DeleteCommand.h"
#include "OnCharCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


OnCharSelectingCommand::OnCharSelectingCommand(TextEditor* textEditor)
	:MacroCommand() {
	this->textEditor = textEditor;
	this->unDoReDo = true;
	this->Add(new DeleteCommand(textEditor, true));
	this->Add(new OnCharCommand(textEditor, true, true));
}

OnCharSelectingCommand::OnCharSelectingCommand(TextEditor* textEditor, bool empty)
	:MacroCommand() {
	this->textEditor = textEditor;
	this->unDoReDo = true;
}

OnCharSelectingCommand::~OnCharSelectingCommand() {

}

Command* OnCharSelectingCommand::EmptyClone() {
	return new OnCharSelectingCommand(this->textEditor, true);
}
