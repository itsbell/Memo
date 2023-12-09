//VSLineDownAction.cpp
#include "VSLineDownAction.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "VerticalScroll.h"
#include "Glyph.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

VSLineDownAction::VSLineDownAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

VSLineDownAction::~VSLineDownAction() {

}

void VSLineDownAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	int distance;
	int position;

	//1. 이동할 거리를 정하다.
	distance = this->textEditor->characterMetrics->GetHeight();
	
	//2. 수직스크롤 포지션을 고치다.
	position = this->textEditor->scrollController->LineDown(distance);

	if (this->textEditor->isScrolling == false) {
		Long vaildMin;
		Long vaildMax;

		vaildMin = this->textEditor->scrollController->GetVaildMin();
		vaildMax = vaildMin + this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight() - this->textEditor->rect.bottom;
		if (vaildMax < 0) {
			vaildMax = 0;
		}
		if (position < vaildMin || position > vaildMax) {
			this->textEditor->SetCurrent();
			this->textEditor->isScrolling = true;
		}
	}
	
	this->textEditor->scrollFlags = SF_NORMAL;
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);

}