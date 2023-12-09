//CtrlShiftUpKeyAction.cpp
#include "CtrlShiftUpKeyAction.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Document.h"
#include "MemoryController.h"
#include "Position.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlShiftUpKeyAction::CtrlShiftUpKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlShiftUpKeyAction::~CtrlShiftUpKeyAction() {

}

void CtrlShiftUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Position previous;
	Position current;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	previous = this->textEditor->document->GetPosition();
	this->textEditor->isMoving = false;
	if (this->textEditor->current->GetCurrent() == 0) {
		if (this->textEditor->note->GetCurrent() > 1) {
			this->textEditor->note->Previous();
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
		else if (this->textEditor->document->GetStart() > 1 && this->textEditor->note->GetCurrent() == 1) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
	}
	this->textEditor->current->First();
	current = this->textEditor->document->GetPosition();	
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);
	this->textEditor->x = 0;
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}