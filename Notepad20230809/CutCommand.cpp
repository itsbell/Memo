//CutCommand.cpp
#include "CutCommand.h"
#include "TextEditor.h"
#include "CopyCommand.h"
#include "DeleteCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CutCommand::CutCommand(TextEditor* textEditor)
	:MacroCommand(){
	this->textEditor = textEditor;
	this->unDoReDo = true;
	this->Add(new CopyCommand(textEditor, true, true));
	this->Add(new DeleteCommand(textEditor, true));
}

CutCommand::CutCommand(TextEditor* textEditor, bool unDoReDo)
	:MacroCommand() {
	this->textEditor = textEditor;
	this->unDoReDo = true;
}

CutCommand::~CutCommand() {

}

Command* CutCommand::EmptyClone() {
	return new CutCommand(this->textEditor, true);
}