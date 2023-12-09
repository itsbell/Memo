//VSThumbTrackACtion.cpp
#include "VSThumbTrackAction.h"
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

VSThumbTrackAction::VSThumbTrackAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

VSThumbTrackAction::~VSThumbTrackAction() {

}

void VSThumbTrackAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	int position;
	Long vaildMin;
	Long vaildMax;
	
	//1. 수직스크롤에서 포지션을 스크롤박스가 놓인 곳으로 이동하다.
	if (nSBCode == SB_THUMBTRACK) { // 16bit값이 들어와 32767을 넘으면 음수값이 들어와 32bit값으로 변경코드
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_TRACKPOS;

		if (GetScrollInfo(this->textEditor->m_hWnd, SB_VERT, &info)) {
			nPos = info.nTrackPos;
		}
	}
	position = this->textEditor->scrollController->VSThumbTrack(nPos);
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