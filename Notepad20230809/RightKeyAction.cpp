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
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void RightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	
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
	//1. ���� ��ġ�� ������ĭ�� �ƴϸ�
	if (this->textEditor->current->GetCurrent() < this->textEditor->current->GetLength()) {
		//1.1. ���� ��ġ�� ���� �ٿ��� ���� ĭ���� �̵��ϴ�.
		this->textEditor->current->Next();
		//1.2. ������ĭ�̰�, ���������� �ƴϸ� �������� DummyRow�̸�
		if (this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength() &&
			this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength() &&
			dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(this->textEditor->note->GetCurrent()))) {
			//1.2.1. ���� �ٷ� �̵��ϴ�.
			this->textEditor->note->Next();
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->current->First();
		}
	}
	//2. ���� ��ġ�� ���������� �ƴϰ� ������ĭ �̸�
	else if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength() &&
			this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {
		//2.1. ���� ��ġ�� ���� �ٷ� �̵��ϴ�.
		this->textEditor->note->Next();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//2.3. ���� ��ġ�� ���� �ٿ��� ó������ �̵��ϴ�.
		this->textEditor->current->First();
	}
	//3. ���� ���� ������ ���������� �ƴϰ�, ���̿��� ������ ���̸� �ٿ��� ������ ĭ�̸�
	else if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength() &&
		this->textEditor->note->GetCurrent() == this->textEditor->note->GetLength() &&
		this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {

		//3.1. �޸� ������Ʈ�� �ȵ� �����̸�
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		//3.2. �޸���Ʈ�ѷ����� �Ʒ��� �̵��ϴ�.
		this->textEditor->memoryController->MoveDown(this->textEditor->document->GetStart() + 1);
		this->textEditor->note->Move(this->textEditor->note->GetCloseRow(this->textEditor->note->GetCurrent()));
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//3.3. �ٿ��� ó������ �̵��ϴ�.
		this->textEditor->current->First();
	}

	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}