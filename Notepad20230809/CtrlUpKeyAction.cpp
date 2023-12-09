//CtrlUpKeyAction.cpp
#include "CtrlUpKeyAction.h"
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

CtrlUpKeyAction::CtrlUpKeyAction() {

}

CtrlUpKeyAction::CtrlUpKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlUpKeyAction::~CtrlUpKeyAction() {

}

/*
함수이름: OnKeyDown
기    능: 적을 위치를 이동하다.
입    력: nChar, nRepCnt, nFlags
출    력: 없음
*/
void CtrlUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long row;

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
	//2. 현재위치가 첫번째 칸이면
	if (this->textEditor->current->GetCurrent() == 0) {
		if (this->textEditor->note->GetCurrent() > 1) {
			//2.1. 이전 줄로 이동하다.
			row = this->textEditor->note->Previous();
			this->textEditor->current = this->textEditor->note->GetAt(row - 1);
		}
		else if(this->textEditor->document->GetStart() > 1 && this->textEditor->note->GetCurrent() == 1){
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
	}
	//3. 적을 위치를 줄에서 첫번째 칸으로 이동하다.
	this->textEditor->current->First();
	//4. 적을 위치 좌표(X)를 고치다.
	this->textEditor->x = 0;
	this->textEditor->y = this->textEditor->GetY();
	//5. 업데이트하다.(스크롤, 캐럿)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//6. 화면을 업데이트하다.
	this->textEditor->Invalidate(FALSE);
}