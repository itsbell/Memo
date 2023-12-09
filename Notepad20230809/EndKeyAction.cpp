//EndKeyAction.cpp
#include "EndKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "Document.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

EndKeyAction::EndKeyAction() {

}

EndKeyAction::EndKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

EndKeyAction::~EndKeyAction() {

}

/*
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {

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
	//2. ���� ��ġ�� ���� �ٿ��� ���������� �̵��ϴ�.
	this->textEditor->current->Last();
	//3. ���� ��ġ�� ��ǥ(X)�� ���ϴ�.
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	//4. ������Ʈ�ϴ�. (��ũ��, ĳ��)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//5. ȭ���� ������Ʈ�ϴ�.
	this->textEditor->Invalidate(FALSE);
}