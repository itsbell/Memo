//HSLineRightAction.h
#ifndef _HSLINERIGHTACTION_H
#define _HSLINERIGHTACTION_H

#include "HScrollAction.h"

class HSLineRightAction : public HScrollAction {
	
	public:
		HSLineRightAction(TextEditor* textEditor);
		virtual ~HSLineRightAction();

		virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#endif //_HSLINERIGHTACTION_H