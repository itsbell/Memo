//LButtonTripleClickedCommand.cpp
#include "LButtonTripleClickedCommand.h"

#include "Document.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "TextEditor.h"

LButtonTripleClickedCommand::LButtonTripleClickedCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

LButtonTripleClickedCommand::~LButtonTripleClickedCommand() {

}

void LButtonTripleClickedCommand::Execute() {

	if (this->textEditor->isContinuous == true) {
		if (this->textEditor->isWrapped == true) {
			this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
		this->textEditor->current->First();
		Position start = this->textEditor->document->GetPosition();
		this->textEditor->current->Last();
		Position end = this->textEditor->document->GetPosition();
		if (this->textEditor->isWrapped == true) {
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, true);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
		this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
		this->textEditor->y = this->textEditor->GetY();
		this->textEditor->document->Select(start, end);
		this->textEditor->document->UpdateSelectingInfo(start, end);
		this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
		this->textEditor->isContinuous = false;
		this->textEditor->Notify();
		this->textEditor->Invalidate(FALSE);
	}
}

void LButtonTripleClickedCommand::Unexecute() {

}