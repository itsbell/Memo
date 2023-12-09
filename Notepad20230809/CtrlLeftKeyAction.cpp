//CtrlLeftKeyAction.cpp
#include "CtrlLeftKeyAction.h"
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

CtrlLeftKeyAction::CtrlLeftKeyAction() {
}

CtrlLeftKeyAction::CtrlLeftKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlLeftKeyAction::~CtrlLeftKeyAction() {
}

/*
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void CtrlLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long previousRow = this->textEditor->note->GetCurrent() - 1;
	
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
	//2. �ڵ��ٹٲ޻����̸� ���� ���� �ڵ��ٹٲ� �ӽ������ϴ�.
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	//3. ù��° ĭ�� �ƴϸ�
	if (this->textEditor->current->GetCurrent() > 0) {
		//3.1. �ٿ��� �����ܾ�� �̵��ϴ�.
		this->textEditor->current->PreviousWord();
		//3.2. �ڵ��ٹٲ� �����̸�
		if (this->textEditor->isWrapped == true) {
			//3.2.2. ���̿��� ���� ���� �ٽ� �ڵ��ٹٲ��ϴ�.
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1,
				this->textEditor->rect.right, this->textEditor->characterMetrics, true);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
	}

	//4. ù��° ���� �ƴϰ� ù��° ĭ�̸�
	else if (this->textEditor->note->GetCurrent() > 1 && this->textEditor->current->GetCurrent() == 0) {	
		//4.1. �ڵ��ٹٲ� �����̸�
		if (this->textEditor->isWrapped == true) {
			//4.1.1. ���̿��� ���� ���� �ٽ� �ڵ��ٹٲ��ϴ�.
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1,
				this->textEditor->rect.right, this->textEditor->characterMetrics, false);
		}
		//4.2. ���� �ٷ� �̵��ϴ�.
		this->textEditor->note->Move(previousRow);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//4.3. �ٿ��� ���������� �̵��ϴ�.
		this->textEditor->current->Last();
	}

	//5. ������ ù��° ���� �ƴϰ�, ������ ù��° ���̸鼭 ù��° ĭ�̸�
	else if (this->textEditor->document->GetStart() > 1 &&
		this->textEditor->note->GetCurrent() == 1 && this->textEditor->current->GetCurrent() == 0) {
		//5.1. �ڵ��ٹٲ� �����̸�
		if (this->textEditor->isWrapped == true) {
			//5.1.1. ���̿��� ���� ���� �ٽ� �ڵ��ٹٲ��ϴ�.
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1,
				this->textEditor->rect.right, this->textEditor->characterMetrics, false);
		}
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->current->Last();
	}
	else {
		if (this->textEditor->isWrapped == true) {
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1,
				this->textEditor->rect.right, this->textEditor->characterMetrics, true);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
	}
	//6. ��ǥ�� ��ġ��.
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	//8. ������Ʈ�ϴ�.
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//9. ȭ���� �����ϴ�.
	this->textEditor->Invalidate(FALSE);
}