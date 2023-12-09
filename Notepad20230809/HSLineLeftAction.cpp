//HSLineLeftAction.cpp
#include "HSLineLeftAction.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HSLineLeftAction::HSLineLeftAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

HSLineLeftAction::~HSLineLeftAction() {

}

void HSLineLeftAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {

	int distance;
	int position;

	//1. �̵��� �Ÿ��� ���ϴ�.
	distance = this->textEditor->characterMetrics->GetWidth(128);

	//2. ����ũ�� �������� ��ġ��.
	position = this->textEditor->scrollController->LineLeft(distance);
	
	this->textEditor->scrollFlags = SF_NORMAL;
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}