//LButtonTripleClickedCommand.cpp
#include "LButtonTripleClickedCommand.h"

#include "Glyph.h"
#include "ScrollController.h"
#include "TextEditor.h"

LButtonTripleClickedCommand::LButtonTripleClickedCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

LButtonTripleClickedCommand::~LButtonTripleClickedCommand() {

}

void LButtonTripleClickedCommand::Execute() {
#if 0
	this->textEditor->current->Move(i);
	this->textEditor->isMoving = false;
	this->textEditor->isScrolling = false;
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
#endif
}

void LButtonTripleClickedCommand::Unexecute() {

}