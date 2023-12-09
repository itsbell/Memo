//HomeKeyAction.cpp
#include "HomeKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Document.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HomeKeyAction::HomeKeyAction() {

}

HomeKeyAction::HomeKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

HomeKeyAction::~HomeKeyAction() {

}

/*
함수이름: OnKeyDown
기    능: 적을 위치를 이동하다.
입    력: nChar, nRepCnt, nFlags
출    력: 없음
*/
void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
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
	//2. 적을 위치를 현재 줄에서 처음으로 이동하다.
	this->textEditor->current->First();
	//3. 적을 위치의 좌표(X)를 구하다.
	this->textEditor->x = 0;
	//4. 업데이트하다. (스크롤, 캐럿)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//5. 화면을 업데이트하다.
	this->textEditor->Invalidate(FALSE);
}