//UpKeyAction.cpp
#include "UpKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
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

UpKeyAction::UpKeyAction() {

}

UpKeyAction::UpKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

UpKeyAction::~UpKeyAction() {

}

/*
함수이름: OnKeyDown
기    능: 적을 위치를 이동하다.
입    력: nChar, nRepCnt, nFlags
출    력: 없음
*/
void UpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long i;
	Long row;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	//2. 선택중 상태이면 선택해제하다.
	if (this->textEditor->document->isSelecting == true) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	//1. 이동중이 아니면
	if (this->textEditor->isMoving == false) {
		this->textEditor->destination = this->textEditor->x;
		this->textEditor->isMoving = true;
	}

	//3. 현재 줄이 문서의 첫번째 줄이 아니면서 종이의 첫번째 줄이면
	if (this->textEditor->document->GetStart() > 1 && this->textEditor->note->GetCurrent() == 1) {	
		//3.1. 메모리 업데이트가 안된 상태이면
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		//3.2. 메모리컨트롤러에서 위로로 이동하다.
		this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//3.3. X좌표를 구하다.
		this->textEditor->x = 0;
		i = 0;
		while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
			this->textEditor->x += this->textEditor->current->GetAt(i)->GetWidth(this->textEditor->characterMetrics);
			i++;
		}
		//3.4. 줄에서 이동하다.
		this->textEditor->current->Move(i);
	}

	//4. 현재줄이 첫 번째 줄이 아니면
	else if (this->textEditor->note->GetCurrent() > 1) {
		//4.1. 현재 줄을 이전 줄로 이동하다.
		row = this->textEditor->note->Previous();
		this->textEditor->current = this->textEditor->note->GetAt(row - 1);
		//4.2. 좌표(X)를 구하다.
		this->textEditor->x = 0;
		i = 0;
		while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
			this->textEditor->x += this->textEditor->current->GetAt(i)->GetWidth(this->textEditor->characterMetrics);
			i++;
		}
		//4.3. 현재 줄에서 칸을 이동하다.
		this->textEditor->current->Move(i);
	}
	
	this->textEditor->y = this->textEditor->GetY();
	//5. 업데이트하다. (스크롤, 캐럿)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();

	//6. 화면을 업데이트하다.
	this->textEditor->Invalidate(FALSE);
}