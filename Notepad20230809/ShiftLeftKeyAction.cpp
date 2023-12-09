//ShiftLeftKeyAction.cpp
#include "ShiftLeftKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "DummyRow.h"
#include "ScrollController.h"
#include "Document.h"
#include "MemoryController.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ShiftLeftKeyAction::ShiftLeftKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ShiftLeftKeyAction::~ShiftLeftKeyAction() {

}

void ShiftLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Position previous;
	Position current;
	
	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	previous = this->textEditor->document->GetPosition();
	this->textEditor->isMoving = false;
	if (this->textEditor->current->GetCurrent() > 0) {
		this->textEditor->current->Previous();
	}
	else if (this->textEditor->note->GetCurrent() > 1 && this->textEditor->current->GetCurrent() == 0) {
		this->textEditor->note->Previous();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->current->Last();
		if (dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(this->textEditor->note->GetCurrent()))) {
			this->textEditor->current->Previous();
		}
	}
	else if (this->textEditor->document->GetStart() > 1 &&
		this->textEditor->note->GetCurrent() == 1 && this->textEditor->current->GetCurrent() == 0) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->current->Last();
	}
	
	current = this->textEditor->document->GetPosition();

	//5. 종이에서 선택하다.
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);
		
	//6. 적을위치를 고치다.
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();

	//7. 업데이트하다.
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();

	//8. 화면을 갱신하다.
	this->textEditor->Invalidate(FALSE);
}