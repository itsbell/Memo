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
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void CtrlDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long row;
	Long column;

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
	//2. ������ ���� �ƴϸ�
	if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength()) {	
		//2.1. ���� �ٷ� �̵��ϴ�.
		row = this->textEditor->note->Next();
		this->textEditor->current = this->textEditor->note->GetAt(row - 1);
		//2.2. ���� ���� ��ǥ(Y)�� ���ϴ�.
		this->textEditor->y = this->textEditor->GetY();
		//2.3. ù��° ĭ���� �̵��ϴ�.
		this->textEditor->current->First();
		this->textEditor->x = 0;
	}
	//3. ������ ���̸�
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
			//3.1. ������ ĭ���� �̵��ϴ�.
			column = this->textEditor->current->Last();
			//3.2. ������ ĭ ��ǥ(X)�� ���ϴ�.
			this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), column);
		}
	}
	//4. ������Ʈ�ϴ�.(��ũ��, ĳ��)
	this->textEditor->scrollFlags = SF_NORMAL;
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//5. ȭ���� ������Ʈ�ϴ�.
	this->textEditor->Invalidate(FALSE);
}