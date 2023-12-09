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
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void LeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	//1. ������ �����̸� ���������ϴ�.
	if (this->textEditor->document->isSelecting == true) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	this->textEditor->isMoving = false;
	//2. ���� ��ġ�� ó���� �ƴϸ�
	if (this->textEditor->current->GetCurrent() > 0) {
		//2.1. ���� ��ġ�� ���� �ٿ��� ���� ĭ���� �̵��ϴ�.
		this->textEditor->current->Previous();	
	}
	//3. ���� ��ġ�� ù° ���� �ƴϰ� �� ó���̸�
	else if (this->textEditor->note->GetCurrent() > 1 && this->textEditor->current->GetCurrent() == 0) {
		//3.1. ���� ��ġ�� ���� �ٷ� �̵��ϴ�.
		this->textEditor->note->Previous();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//3.3. ���� ��ġ�� ������ĭ���� �̵��ϴ�.
		this->textEditor->current->Last();
		//3.4. DummyRow ������ ����ĭ���� �̵��ϴ�.
		if (dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(this->textEditor->note->GetCurrent()))) {
			this->textEditor->current->Previous();
		}
	}
	//4. �������� ������ ù° ���� �ƴϰ�, ������ ù° ���̸鼭 ù��° ĭ�̸�
	else if (this->textEditor->document->GetStart() > 1 &&
		this->textEditor->note->GetCurrent() == 1 && this->textEditor->current->GetCurrent() == 0) {
		//4.1. �޸� ������Ʈ�� �ȵ� �����̸�
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		//4.2. �޸���Ʈ�ѷ����� ���� �̵��ϴ�.
		this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//4.3. �ٿ��� ���������� �̵��ϴ�.
		this->textEditor->current->Last();
	}
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	//5. ������Ʈ�ϴ�.(��ũ��, ĳ��)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//6. ȭ���� ������Ʈ�ϴ�.
	this->textEditor->Invalidate(FALSE);
}