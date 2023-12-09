//HSPageLeftAction.h
#ifndef _HSPAGELEFTACTION_H
#define _HSPAGELEFTACTION_H

#include "HScrollAction.h"

class HSPageLeftAction : public HScrollAction {
	
	public:
		HSPageLeftAction(TextEditor* textEditor);
		virtual ~HSPageLeftAction();

		virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#endif //_HSPAGELEFTACTION_H
