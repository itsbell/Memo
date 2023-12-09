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
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void CtrlRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	//1. ������ �����̸� ���̿��� ���������ϴ�.
	if (this->textEditor->document->isSelecting == true) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	//7. �̵��� ���¸� ��ġ��.
	this->textEditor->isMoving = false;
	//2. �ڵ��ٹٲ� �����̸� ���̿��� ���� ���� �ڵ��ٹٲ� �ӽ������ϴ�.
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	//3. ������ ĭ�� �ƴϸ�
	if (this->textEditor->current->GetCurrent() < this->textEditor->current->GetLength()) {
		//3.1. �ٿ��� �����ܾ�� �̵��ϴ�.
		this->textEditor->current->NextWord();
		//3.2. �ڵ��ٹٲ� �����̸�
		if (this->textEditor->isWrapped == true) {
			//3.2.2. ���̿��� ���� ���� �ڵ��ٹٲ��ϴ�.
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, true);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
	}
	//4. ������ ���� �ƴϰ� ������ ĭ�̸�
	else if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength()	&& this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {
		//4.1. �ڵ��ٹٲ� �����̸� ���̿��� ���� ���� �ٽ� �ڵ��ٹٲ��ϴ�.
		if (this->textEditor->isWrapped == true) {
			//4.1.1. ���̿��� ���� ���� �ٽ� �ڵ��ٹٲ��ϴ�.
			this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
		}
		//4.2. ���� �ٷ� �̵��ϴ�.
		index = this->textEditor->note->Next();
		this->textEditor->current = this->textEditor->note->GetAt(index - 1);
		//4.3. ù��° ĭ���� �̵��ϴ�.
		this->textEditor->current->First();
	}
	// ������ ������ ���� �ƴϰ�, ������ ������ ���̸鼭 �ٿ��� ������ ĭ�̸�
	else if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength() &&
			this->textEditor->note->GetCurrent() == this->textEditor->note->GetLength() &&
			this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {
		//5.1. �ڵ��ٹٲ� �����̸�
		if (this->textEditor->isWrapped == true) {
			//5.1.1. ���̿��� ���� ���� �ٽ� �ڵ��ٹٲ��ϴ�.
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

	//6. ������ġ�� ��ġ��.
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	//8. ������Ʈ�ϴ�. (��ũ��, ĳ��)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//9. ȭ���� ������Ʈ�ϴ�.
	this->textEditor->Invalidate(FALSE);
}