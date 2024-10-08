//SelectingLeftKeyAction.cpp
#include "SelectingLeftKeyAction.h"

#include "Document.h"
#include "FileMaker.h"
#include "Glyph.h"
#include "MemoryController.h"
#include "resource.h"
#include "ScrollController.h"
#include "TextEditor.h"

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
	FileMaker fileMaker(this->textEditor); //Log 积己内靛
	Long row = this->textEditor->document->startPosition.row;
	Long column = this->textEditor->document->startPosition.column;

	fileMaker.RecordLog("Paper::UnSelect", true); //Log 积己内靛
	this->textEditor->note->UnSelect(fileMaker.GetLog());
	this->textEditor->document->isSelecting = false;
	if (this->textEditor->document->GetStart() > row || this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1 < row) {
		if (this->textEditor->isUpdated == false) {
			fileMaker.RecordLog("MemoryController::Save", true); //Log 积己内靛
			this->textEditor->memoryController->Save();
			fileMaker.RecordLog("ScrollController::UpdateFileVSInfo", true); //Log 积己内靛
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		if (this->textEditor->document->GetStart() > row) {
			fileMaker.RecordLog("MemoryController::MoveUp", true); //Log 积己内靛
			this->textEditor->memoryController->MoveUp(row);
		}
		else {
			fileMaker.RecordLog("MemoryController::MoveDown", true); //Log 积己内靛
			this->textEditor->memoryController->MoveDown(row - this->textEditor->note->GetRowCount() + 1);
		}
	}
	fileMaker.RecordLog("Document::Move", true); //Log 积己内靛
	this->textEditor->document->Move(row, column, fileMaker.GetLog());
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->isMoving = false;
	this->textEditor->isScrolling = false;
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	fileMaker.RecordLog("ScrollController::UpdatePosition", true); //Log 积己内靛
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	fileMaker.RecordLog("TextEditor::Notify", true); //Log 积己内靛
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}