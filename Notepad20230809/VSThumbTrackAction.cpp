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
	
	//1. ������ũ�ѿ��� �������� ��ũ�ѹڽ��� ���� ������ �̵��ϴ�.
	if (nSBCode == SB_THUMBTRACK) { // 16bit���� ���� 32767�� ������ �������� ���� 32bit������ �����ڵ�
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