//VSPageDownAction.cpp
#include "VSPageDownAction.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "VerticalScroll.h"
#include "CharacterMetrics.h"
#include "Glyph.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

VSPageDownAction::VSPageDownAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

VSPageDownAction::~VSPageDownAction() {

}

void VSPageDownAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	int position;
	Long vaildMin;
	Long vaildMax;

	//2. 수직스크롤 포지션을 한 페이지만큼 아래로 이동하다.
	position = this->textEditor->scrollController->PageDown(this->textEditor->rect.bottom);
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