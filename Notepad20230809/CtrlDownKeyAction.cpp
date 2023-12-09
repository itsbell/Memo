//CtrlDownKeyAction.cpp
#include "CtrlDownKeyAction.h"
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

CtrlDownKeyAction::CtrlDownKeyAction() {

}

CtrlDownKeyAction::CtrlDownKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlDownKeyAction::~CtrlDownKeyAction() {

}

/*
함수이름: OnKeyDown
기    능: 적을 위치를 이동하다.
입    력: nChar, nRepCnt, nFlags
출    력: 없음
*/
void CtrlDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long row;
	Long column;

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
	//2. 마지막 줄이 아니면
	if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength()) {	
		//2.1. 다음 줄로 이동하다.
		row = this->textEditor->note->Next();
		this->textEditor->current = this->textEditor->note->GetAt(row - 1);
		//2.2. 다음 줄의 좌표(Y)를 구하다.
		this->textEditor->y = this->textEditor->GetY();
		//2.3. 첫번째 칸으로 이동하다.
		this->textEditor->current->First();
		this->textEditor->x = 0;
	}
	//3. 마지막 줄이면
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
			this->textEditor->x = 0;
			this->textEditor->y = this->textEditor->GetY();
		}
		else {
			//3.1. 마지막 칸으로 이동하다.
			column = this->textEditor->current->Last();
			//3.2. 마지막 칸 좌표(X)를 구하다.
			this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), column);
		}
	}
	//4. 업데이트하다.(스크롤, 캐럿)
	this->textEditor->scrollFlags = SF_NORMAL;
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//5. 화면을 업데이트하다.
	this->textEditor->Invalidate(FALSE);
}