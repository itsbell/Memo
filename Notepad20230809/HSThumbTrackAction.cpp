//HSThumbTrackAction.cpp
#include "HSThumbTrackAction.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "HorizontalScroll.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HSThumbTrackAction::HSThumbTrackAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

HSThumbTrackAction::~HSThumbTrackAction() {

}

void HSThumbTrackAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {

	int position;
	
	//1. 수평스크롤 포지션을 스크롤박스가 놓인 곳으로 이동하다.

	if (nSBCode == SB_THUMBTRACK) { // 16bit값이 들어와 32767을 넘으면 음수값이 들어와 32bit값으로 변경코드
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_TRACKPOS;

		if (GetScrollInfo(this->textEditor->m_hWnd, SB_HORZ, &info)) {
			nPos = info.nTrackPos;
		}
	}

	position = this->textEditor->scrollController->HSThumbTrack(nPos);
	this->textEditor->scrollFlags = SF_NORMAL;
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}