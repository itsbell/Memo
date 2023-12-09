//HSPageRightAction.h
#ifndef _HSPAGERIGHTACTION_H
#define _HSPAGERIGHTACTION_H

#include "HScrollAction.h"

class HSPageRightAction : public HScrollAction {

	public:
		HSPageRightAction(TextEditor* textEditor);
		virtual ~HSPageRightAction();

		virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#endif //_HSPAGERIGHTACTION_H