//LeftKeyAction.cpp
#include <string>
using namespace std;
#include "LeftKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "DummyRow.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "HorizontalScroll.h"
#include "VerticalScroll.h"
#include "Document.h"
#include "MemoryController.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LeftKeyAction::LeftKeyAction() {
}

LeftKeyAction::LeftKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

LeftKeyAction::~LeftKeyAction() {
}

/*
함수이름: OnKeyDown
기    능: 적을 위치를 이동하다.
입    력: nChar, nRepCnt, nFlags
출    력: 없음
*/
void LeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {

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
	//2. 적을 위치가 처음이 아니면
	if (this->textEditor->current->GetCurrent() > 0) {
		//2.1. 적을 위치를 현재 줄에서 이전 칸으로 이동하다.
		this->textEditor->current->Previous();	
	}
	//3. 적을 위치가 첫째 줄이 아니고 맨 처음이면
	else if (this->textEditor->note->GetCurrent() > 1 && this->textEditor->current->GetCurrent() == 0) {
		//3.1. 적을 위치를 이전 줄로 이동하다.
		this->textEditor->note->Previous();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//3.3. 적을 위치를 마지막칸으로 이동하다.
		this->textEditor->current->Last();
		//3.4. DummyRow 였으면 이전칸으로 이동하다.
		if (dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(this->textEditor->note->GetCurrent()))) {
			this->textEditor->current->Previous();
		}
	}
	//4. 현재줄이 문서의 첫째 줄이 아니고, 종이의 첫째 줄이면서 첫번째 칸이면
	else if (this->textEditor->document->GetStart() > 1 &&
		this->textEditor->note->GetCurrent() == 1 && this->textEditor->current->GetCurrent() == 0) {
		//4.1. 메모리 업데이트가 안된 상태이면
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		//4.2. 메모리컨트롤러에서 위로 이동하다.
		this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//4.3. 줄에서 마지막으로 이동하다.
		this->textEditor->current->Last();
	}
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	//5. 업데이트하다.(스크롤, 캐럿)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//6. 화면을 업데이트하다.
	this->textEditor->Invalidate(FALSE);
}