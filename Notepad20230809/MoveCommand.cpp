//MoveCommand.cpp
#include "MoveCommand.h"
#include "TextEditor.h"
#include "MoveForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

MoveCommand::MoveCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

MoveCommand::~MoveCommand() {

}

void MoveCommand::Execute() {
	MoveForm moveForm(this->textEditor, NULL);

	moveForm.DoModal();
}

void MoveCommand::Unexecute() {

}