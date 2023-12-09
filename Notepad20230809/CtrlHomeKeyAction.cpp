//CtrlHomeKeyAction.cpp
#include "CtrlHomeKeyAction.h"
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

CtrlHomeKeyAction::CtrlHomeKeyAction() {

}

CtrlHomeKeyAction::CtrlHomeKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlHomeKeyAction::~CtrlHomeKeyAction() {

}

/*
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	//1. ���û����̸� ���������ϴ�.
	if (this->textEditor->document->isSelecting == true) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	this->textEditor->isMoving = false;
	if (this->textEditor->document->GetStart() > 1) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveUp(1);
	}

	//2. ù��° �ٷ� �̵��ϴ�.
	this->textEditor->note->First();
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent()-1);
	//3. ù��° ĭ���� �̵��ϴ�.
	this->textEditor->current->First();
	this->textEditor->x = 0;
	this->textEditor->y = 0;
	//5. ������Ʈ�ϴ�.
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//6. ȭ���� ������Ʈ�ϴ�.
	this->textEditor->Invalidate(FALSE);
}