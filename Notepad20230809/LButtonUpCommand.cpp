//LButtonUpCommand.cpp
#include "LButtonUpCommand.h"
#include "TextEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif
LButtonUpCommand::LButtonUpCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

LButtonUpCommand::~LButtonUpCommand() {

}

void LButtonUpCommand::Execute() {
	::ReleaseCapture();
	this->textEditor->isClicking = false;
}

void LButtonUpCommand::Unexecute() {

}