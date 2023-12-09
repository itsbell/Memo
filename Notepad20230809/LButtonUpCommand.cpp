//LButtonUpCommand.cpp
#include "LButtonUpCommand.h"
#include "TextEditor.h"

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