//CtrlHomeKeyAction.cpp
#include "CtrlHomeKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Document.h"
#include "MemoryController.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlHomeKeyAction::CtrlHomeKeyAction() {

}

CtrlHomeKeyAction::CtrlHomeKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlHomeKeyAction::~CtrlHomeKeyAction() {

}

/*
함수이름: OnKeyDown
기    능: 적을 위치를 이동하다.
입    력: nChar, nRepCnt, nFlags
출    력: 없음
*/
void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	//1. 선택상태이면 선택해제하다.
	if (this->textEditor->document->isSelecting == true) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	this->textEditor->isMoving = false;
	if (this->textEditor->document->GetStart() > 1) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveUp(1);
	}

	//2. 첫번째 줄로 이동하다.
	this->textEditor->note->First();
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent()-1);
	//3. 첫번째 칸으로 이동하다.
	this->textEditor->current->First();
	this->textEditor->x = 0;
	this->textEditor->y = 0;
	//5. 업데이트하다.
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//6. 화면을 업데이트하다.
	this->textEditor->Invalidate(FALSE);
}