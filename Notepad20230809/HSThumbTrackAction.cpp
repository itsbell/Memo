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
	
	//1. ����ũ�� �������� ��ũ�ѹڽ��� ���� ������ �̵��ϴ�.

	if (nSBCode == SB_THUMBTRACK) { // 16bit���� ���� 32767�� ������ �������� ���� 32bit������ �����ڵ�
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