//HSPageRightAction.cpp
#include "HSPageRightAction.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "HorizontalScroll.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HSPageRightAction::HSPageRightAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

HSPageRightAction::~HSPageRightAction() {

}

void HSPageRightAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	int position;

	//2. ����ũ�ѿ��� �������� ���������� �� ��������ŭ �̵��ϴ�.
	position = this->textEditor->scrollController->PageRight(this->textEditor->rect.right);

	this->textEditor->scrollFlags = SF_NORMAL;
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
	
}