//LButtonDblClkCommand.cpp
#include "LButtonDblClkCommand.h"

#include "Document.h"
#include "Glyph.h"
#include "Position.h"
#include "ScrollController.h"
#include "TextEditor.h"

LButtonDblClkCommand::LButtonDblClkCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

LButtonDblClkCommand::~LButtonDblClkCommand()
{
}

void LButtonDblClkCommand::Execute() {
	this->textEditor->current->PreviousWord();
	Position start = this->textEditor->document->GetPosition();
	this->textEditor->current->NextWord();
	Position end = this->textEditor->document->GetPosition();
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->document->Select(start, end);
	this->textEditor->document->UpdateSelectingInfo(start, end);
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}

void LButtonDblClkCommand::Unexecute()
{
}
