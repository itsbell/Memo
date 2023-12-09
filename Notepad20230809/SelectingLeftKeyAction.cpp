//SelectingLeftKeyAction.cpp
#include "SelectingLeftKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Document.h"
#include "MemoryController.h"
#include "resource.h"
#include "Position.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SelectingLeftKeyAction::SelectingLeftKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

SelectingLeftKeyAction::~SelectingLeftKeyAction() {

}

void SelectingLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long row = this->textEditor->document->startPosition.row;
	Long column = this->textEditor->document->startPosition.column;

	this->textEditor->note->UnSelect();
	this->textEditor->document->isSelecting = false;
	if (this->textEditor->document->GetStart() > row || this->textEditor->document->GetEnd() < row) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		if (this->textEditor->document->GetStart() > row) {
			this->textEditor->memoryController->MoveUp(row);
		}
		else {
			this->textEditor->memoryController->MoveDown(row - this->textEditor->note->GetRowCount() + 1);
		}
	}
	this->textEditor->document->Move(row, column);
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->isMoving = false;
	this->textEditor->isScrolling = false;
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}