//CtrlShiftLeftKeyAction.cpp
#include "CtrlShiftLeftKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "DummyRow.h"
#include "Document.h"
#include "Position.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlShiftLeftKeyAction::CtrlShiftLeftKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlShiftLeftKeyAction::~CtrlShiftLeftKeyAction() {

}

void CtrlShiftLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Position previous;
	Position current;
	
	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	previous = this->textEditor->document->GetPosition();
	this->textEditor->isMoving = false;
	//1. �ڵ��ٹٲ޻����̸� ���̿��� ���� ���� �ڵ��ٹٲ� �ӽ������ϴ�.
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	//2. ù��° ĭ�� �ƴϸ� �ٿ��� �����ܾ�� �̵��ϴ�.
	if (this->textEditor->current->GetCurrent() > 0) {
		//2.1. �ٿ��� �����ܾ�� �̵��ϴ�.
		this->textEditor->current->PreviousWord();
	}
	//3. ù��° ĭ�̸�
	else{
		if (this->textEditor->isWrapped == true) {
			this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
		}
		if (this->textEditor->note->GetCurrent() > 1) {
			this->textEditor->note->Previous();
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->current->Last();
		}
		else if(this->textEditor->note->GetCurrent() == 1 && this->textEditor->document->GetStart() > 1) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->current->Last();
		}
	}
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	current = this->textEditor->document->GetPosition();
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}