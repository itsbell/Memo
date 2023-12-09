//RightKeyAction.cpp
#include "RightKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "DummyRow.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "Document.h"
#include "MemoryController.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RightKeyAction::RightKeyAction() {
}

RightKeyAction::RightKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

RightKeyAction::~RightKeyAction() {
}

/*
함수이름: OnKeyDown
기    능: 적을 위치를 이동하다.
입    력: nChar, nRepCnt, nFlags
출    력: 없음
*/
void RightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	
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
	//1. 적을 위치가 마지막칸이 아니면
	if (this->textEditor->current->GetCurrent() < this->textEditor->current->GetLength()) {
		//1.1. 적을 위치를 현재 줄에서 다음 칸으로 이동하다.
		this->textEditor->current->Next();
		//1.2. 마지막칸이고, 마지막줄이 아니며 다음줄이 DummyRow이면
		if (this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength() &&
			this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength() &&
			dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(this->textEditor->note->GetCurrent()))) {
			//1.2.1. 다음 줄로 이동하다.
			this->textEditor->note->Next();
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->current->First();
		}
	}
	//2. 적을 위치가 마지막줄이 아니고 마지막칸 이면
	else if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength() &&
			this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {
		//2.1. 적을 위치를 다음 줄로 이동하다.
		this->textEditor->note->Next();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//2.3. 적을 위치를 현재 줄에서 처음으로 이동하다.
		this->textEditor->current->First();
	}
	//3. 현재 줄이 문서의 마지막줄이 아니고, 종이에서 마지막 줄이며 줄에서 마지막 칸이면
	else if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength() &&
		this->textEditor->note->GetCurrent() == this->textEditor->note->GetLength() &&
		this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {

		//3.1. 메모리 업데이트가 안된 상태이면
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		//3.2. 메모리컨트롤러에서 아래로 이동하다.
		this->textEditor->memoryController->MoveDown(this->textEditor->document->GetStart() + 1);
		this->textEditor->note->Move(this->textEditor->note->GetCloseRow(this->textEditor->note->GetCurrent()));
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//3.3. 줄에서 처음으로 이동하다.
		this->textEditor->current->First();
	}

	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}