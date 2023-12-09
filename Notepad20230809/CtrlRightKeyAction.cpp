//CtrlRightKeyAction.cpp
#include "CtrlRightKeyAction.h"
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

CtrlRightKeyAction::CtrlRightKeyAction() {

}

CtrlRightKeyAction::CtrlRightKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlRightKeyAction::~CtrlRightKeyAction() {

}

/*
함수이름: OnKeyDown
기    능: 적을 위치를 이동하다.
입    력: nChar, nRepCnt, nFlags
출    력: 없음
*/
void CtrlRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	//1. 선택중 상태이면 종이에서 선택해제하다.
	if (this->textEditor->document->isSelecting == true) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	//7. 이동중 상태를 고치다.
	this->textEditor->isMoving = false;
	//2. 자동줄바꿈 상태이면 종이에서 현재 줄을 자동줄바꿈 임시해제하다.
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	//3. 마지막 칸이 아니면
	if (this->textEditor->current->GetCurrent() < this->textEditor->current->GetLength()) {
		//3.1. 줄에서 다음단어로 이동하다.
		this->textEditor->current->NextWord();
		//3.2. 자동줄바꿈 상태이면
		if (this->textEditor->isWrapped == true) {
			//3.2.2. 종이에서 현재 줄을 자동줄바꿈하다.
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, true);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
	}
	//4. 마지막 줄이 아니고 마지막 칸이면
	else if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength()	&& this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {
		//4.1. 자동줄바꿈 상태이면 종이에서 현재 줄을 다시 자동줄바꿈하다.
		if (this->textEditor->isWrapped == true) {
			//4.1.1. 종이에서 현재 줄을 다시 자동줄바꿈하다.
			this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
		}
		//4.2. 다음 줄로 이동하다.
		index = this->textEditor->note->Next();
		this->textEditor->current = this->textEditor->note->GetAt(index - 1);
		//4.3. 첫번째 칸으로 이동하다.
		this->textEditor->current->First();
	}
	// 문서의 마지막 줄이 아니고, 종이의 마지막 줄이면서 줄에서 마지막 칸이면
	else if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength() &&
			this->textEditor->note->GetCurrent() == this->textEditor->note->GetLength() &&
			this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {
		//5.1. 자동줄바꿈 상태이면
		if (this->textEditor->isWrapped == true) {
			//5.1.1. 종이에서 현재 줄을 다시 자동줄바꿈하다.
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, false);
		}
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveDown(this->textEditor->document->GetStart() + 1);
		this->textEditor->note->Move(this->textEditor->note->GetCloseRow(this->textEditor->note->GetCurrent()));
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->current->First();
	}
	else {
		if (this->textEditor->isWrapped == true) {
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1,
				this->textEditor->rect.right, this->textEditor->characterMetrics, true);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
	}

	//6. 적을위치를 고치다.
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	//8. 업데이트하다. (스크롤, 캐럿)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//9. 화면을 업데이트하다.
	this->textEditor->Invalidate(FALSE);
}