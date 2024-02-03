//LButtonTripleClickedCommand.cpp
#include "LButtonTripleClickedCommand.h"

LButtonTripleClickedCommand::LButtonTripleClickedCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

LButtonTripleClickedCommand::~LButtonTripleClickedCommand() {

}

void LButtonTripleClickedCommand::Execute() {
	int a = 5;
	int c = 5;
	int b = 5;
}

void LButtonTripleClickedCommand::Unexecute() {

}