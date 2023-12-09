//SelectingRightKeyAction.cpp
#include "SelectingRightKeyAction.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Document.h"
#include "MemoryController.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SelectingRightKeyAction::SelectingRightKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

SelectingRightKeyAction::~SelectingRightKeyAction() {

}

void SelectingRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long row = this->textEditor->document->endPosition.row;
	Long column = this->textEditor->document->endPosition.column;

	this->textEditor->note->UnSelect();
	this->textEditor->document->isSelecting = false;
	if (this->textEditor->document->GetEnd() < row || this->textEditor->document->GetStart() > row) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		if(this->textEditor->document->GetEnd() < row){
			this->textEditor->memoryController->MoveDown(row - this->textEditor->note->GetRowCount() + 1);
			
		}
		else {
			this->textEditor->memoryController->MoveUp(row);
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