//CtrlEndKeyAction.cpp
#include "CtrlEndKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Document.h"
#include "MemoryController.h"
#include "resource.h"
#include "CharacterMetrics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlEndKeyAction::CtrlEndKeyAction() {

}

CtrlEndKeyAction::CtrlEndKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlEndKeyAction::~CtrlEndKeyAction() {

}

/*
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
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
	if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength()) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveDown(this->textEditor->document->GetLength() - this->textEditor->note->GetRowCount() + 1);
	}
	this->textEditor->note->Last();
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->current->Last();

	//4. ��ǥ�� ���ϴ�.
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	//5. ������Ʈ�ϴ�.(��ũ��, ĳ��)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//6. ȭ���� ������Ʈ�ϴ�.
	this->textEditor->Invalidate(FALSE);
}