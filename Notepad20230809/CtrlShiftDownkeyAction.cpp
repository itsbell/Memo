//CtrlShiftDownKeyAction.cpp
#include "CtrlShiftDownKeyAction.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Document.h"
#include "Position.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlShiftDownKeyAction::CtrlShiftDownKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlShiftDownKeyAction::~CtrlShiftDownKeyAction() {

}

void CtrlShiftDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Position previous;
	Position current;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	previous = this->textEditor->document->GetPosition();
	this->textEditor->isMoving = false;
	if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength()) {
		this->textEditor->note->Next();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->current->First();
	}
	else {
		if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength()) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			this->textEditor->memoryController->MoveDown(this->textEditor->document->GetStart() + 1);
			this->textEditor->note->Move(this->textEditor->note->GetCloseRow(this->textEditor->note->GetCurrent()));
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->current->First();
		}
		else {
			this->textEditor->current->Last();
		}
	}
	current = this->textEditor->document->GetPosition();
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}