//LButtonDblClkCommand.cpp
#include "LButtonDblClkCommand.h"

#include "Document.h"
#include "Glyph.h"
#include "Position.h"
#include "ScrollController.h"
#include "TextEditor.h"
#include "Time.h"

LButtonDblClkCommand::LButtonDblClkCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

LButtonDblClkCommand::~LButtonDblClkCommand()
{
}

void LButtonDblClkCommand::Execute() {
	
	*(this->textEditor->doubleClickTime) = Time::GetCurrent();
	
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	this->textEditor->current->PreviousWord();
	Position start = this->textEditor->document->GetPosition();
	this->textEditor->current->NextWord();
	Position end = this->textEditor->document->GetPosition();
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, true);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	this->textEditor->isContinuous = true;
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
