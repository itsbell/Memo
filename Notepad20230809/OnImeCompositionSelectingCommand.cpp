//OnImeCompositionSelectingCommand.cpp
#include "OnImeCompositionSelectingCommand.h"
#include "TextEditor.h"
#include "Command.h"
#include "DeleteCommand.h"
#include "OnImeCompositionCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OnImeCompositionSelectingCommand::OnImeCompositionSelectingCommand(TextEditor* textEditor)
	:MacroCommand() {

	this->textEditor = textEditor;
	this->unDoReDo = true;

	this->Add(new DeleteCommand(textEditor, true));
	
	this->Add(new OnImeCompositionCommand(textEditor, true, true));
}

OnImeCompositionSelectingCommand::OnImeCompositionSelectingCommand(TextEditor* textEditor, bool unDoReDo)
	:MacroCommand() {
	this->textEditor = textEditor;
	this->unDoReDo = true;
}

OnImeCompositionSelectingCommand::~OnImeCompositionSelectingCommand() {

}

Command* OnImeCompositionSelectingCommand::EmptyClone() {
	return new OnImeCompositionSelectingCommand(this->textEditor, true);
}