//CtrlUpKeyAction.cpp
#include "CtrlUpKeyAction.h"
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

CtrlUpKeyAction::CtrlUpKeyAction() {

}

CtrlUpKeyAction::CtrlUpKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlUpKeyAction::~CtrlUpKeyAction() {

}

/*
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void CtrlUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long row;

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
	//2. ������ġ�� ù��° ĭ�̸�
	if (this->textEditor->current->GetCurrent() == 0) {
		if (this->textEditor->note->GetCurrent() > 1) {
			//2.1. ���� �ٷ� �̵��ϴ�.
			row = this->textEditor->note->Previous();
			this->textEditor->current = this->textEditor->note->GetAt(row - 1);
		}
		else if(this->textEditor->document->GetStart() > 1 && this->textEditor->note->GetCurrent() == 1){
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
	}
	//3. ���� ��ġ�� �ٿ��� ù��° ĭ���� �̵��ϴ�.
	this->textEditor->current->First();
	//4. ���� ��ġ ��ǥ(X)�� ��ġ��.
	this->textEditor->x = 0;
	this->textEditor->y = this->textEditor->GetY();
	//5. ������Ʈ�ϴ�.(��ũ��, ĳ��)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//6. ȭ���� ������Ʈ�ϴ�.
	this->textEditor->Invalidate(FALSE);
}