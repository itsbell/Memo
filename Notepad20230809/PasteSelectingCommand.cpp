//PasteSelectingCommand.cpp
#include "PasteSelectingCommand.h"
#include "DeleteCommand.h"
#include "PasteCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PasteSelectingCommand::PasteSelectingCommand(TextEditor* textEditor)
	:MacroCommand() {
	this->textEditor = textEditor;
	this->unDoReDo = true;

	this->Add(new DeleteCommand(textEditor, true));
	this->Add(new PasteCommand(textEditor, true, true));
}

PasteSelectingCommand::PasteSelectingCommand(TextEditor* textEditor, bool unDoReDo)
	:MacroCommand() {
	this->textEditor = textEditor;
	this->unDoReDo = true;
}

PasteSelectingCommand::~PasteSelectingCommand() {

}

Command* PasteSelectingCommand::EmptyClone() {
	return new PasteSelectingCommand(this->textEditor, true);
}