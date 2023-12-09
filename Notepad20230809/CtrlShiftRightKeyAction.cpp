//CtrlShiftRightKeyAction.cpp
#include "CtrlShiftRightKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Document.h"
#include "Position.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlShiftRightKeyAction::CtrlShiftRightKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlShiftRightKeyAction::~CtrlShiftRightKeyAction() {

}

void CtrlShiftRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Position previous;
	Position current;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	previous = this->textEditor->document->GetPosition();
	this->textEditor->isMoving = false;
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	if (this->textEditor->current->GetCurrent() < this->textEditor->current->GetLength()) {
		this->textEditor->current->NextWord();
	}
	else {
		if (this->textEditor->isWrapped == true) {
			this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
		}
		if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength()) {
			this->textEditor->note->Next();
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->current->First();
		}
		else if (this->textEditor->note->GetCurrent() == this->textEditor->note->GetLength() &&
			this->textEditor->document->GetEnd() < this->textEditor->document->GetLength()) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			this->textEditor->memoryController->MoveDown(this->textEditor->document->GetStart() + 1);
			this->textEditor->note->Move(this->textEditor->note->GetCloseRow(this->textEditor->note->GetCurrent()));
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->current->First();
		}
	}
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	//5. 종이에서 선택하다.
	current = this->textEditor->document->GetPosition();
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}
