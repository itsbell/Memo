//CtrlShiftHomeKeyAction.cpp
#include "CtrlShiftHomeKeyAction.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Position.h"
#include "Document.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlShiftHomeKeyAction::CtrlShiftHomeKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlShiftHomeKeyAction::~CtrlShiftHomeKeyAction() {

}

void CtrlShiftHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Position previous;
	Position current;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	previous = this->textEditor->document->GetPosition();
	this->textEditor->isMoving = false;
	if (this->textEditor->document->GetStart() > 1) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveUp(1);
	}
	this->textEditor->note->First();
	this->textEditor->current = this->textEditor->note->GetAt(0);
	this->textEditor->current->First();
	this->textEditor->x = 0;
	this->textEditor->y = 0;
	
	current = this->textEditor->document->GetPosition();
	//1.5. 종이에서 선택하다.
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);

	//1.6. 스크롤포지션을 업데이트하다.
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);

	//1.7. 업데이트하다.
	this->textEditor->Notify();

	//1.8. 화면을 갱신하다.
	this->textEditor->Invalidate(FALSE);
	
}