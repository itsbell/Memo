//HLineRightAction.cpp
#include "HSLineRightAction.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "HorizontalScroll.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HSLineRightAction::HSLineRightAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

HSLineRightAction::~HSLineRightAction() {

}

void HSLineRightAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {

	int position;
	int distance;

	//1. �̵��� �Ÿ��� ���ϴ�.
	distance = this->textEditor->characterMetrics->GetWidth(128);

	//2. ����ũ�� �������� ��ġ��.
	position = this->textEditor->scrollController->LineRight(distance);

	this->textEditor->scrollFlags = SF_NORMAL;
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);

}