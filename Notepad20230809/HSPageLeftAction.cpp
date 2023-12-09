//HSPageLeftAction.cpp
#include "HSPageLeftAction.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "HorizontalScroll.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HSPageLeftAction::HSPageLeftAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

HSPageLeftAction::~HSPageLeftAction() {

}

void HSPageLeftAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	int position;
	
	//2. 수평스크롤 포지션을 왼쪽으로 한 페이지만큼 이동하다.
	position = this->textEditor->scrollController->PageLeft(this->textEditor->rect.right);
	this->textEditor->scrollFlags = SF_NORMAL;
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);

}