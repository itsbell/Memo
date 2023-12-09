//CtrlEndKeyAction.cpp
#include "CtrlEndKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Document.h"
#include "MemoryController.h"
#include "resource.h"
#include "CharacterMetrics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlEndKeyAction::CtrlEndKeyAction() {

}

CtrlEndKeyAction::CtrlEndKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlEndKeyAction::~CtrlEndKeyAction() {

}

/*
함수이름: OnKeyDown
기    능: 적을 위치를 이동하다.
입    력: nChar, nRepCnt, nFlags
출    력: 없음
*/
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	//1. 선택중 상태이면 선택해제하다.
	if (this->textEditor->document->isSelecting == true) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	this->textEditor->isMoving = false;
	if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength()) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveDown(this->textEditor->document->GetLength() - this->textEditor->note->GetRowCount() + 1);
	}
	this->textEditor->note->Last();
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->current->Last();

	//4. 좌표를 구하다.
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	//5. 업데이트하다.(스크롤, 캐럿)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//6. 화면을 업데이트하다.
	this->textEditor->Invalidate(FALSE);
}