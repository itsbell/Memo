//OnCharEnterSelectingCommand.cpp
#include "OnCharEnterSelectingCommand.h"
#include "TextEditor.h"
#include "DeleteCommand.h"
#include "OnCharEnterCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


OnCharEnterSelectingCommand::OnCharEnterSelectingCommand(TextEditor* textEditor)
	:MacroCommand() {
	Command* command;
	
	this->textEditor = textEditor;
	this->unDoReDo = true;

	this->Add(new DeleteCommand(textEditor, true));

	this->Add(new OnCharEnterCommand(textEditor, true));
}

OnCharEnterSelectingCommand::OnCharEnterSelectingCommand(TextEditor* textEditor, bool unDoReDo)
	:MacroCommand() {
	this->textEditor = textEditor;
	this->unDoReDo = true;
}

OnCharEnterSelectingCommand::~OnCharEnterSelectingCommand() {

}

Command* OnCharEnterSelectingCommand::EmptyClone() {
	return new OnCharEnterSelectingCommand(this->textEditor, true);
}
