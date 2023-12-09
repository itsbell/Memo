//HSLineLeftAction.h
#ifndef _HLINELEFTACTION_H
#define _HLINELEFTACTION_H

#include "HScrollAction.h"

class HSLineLeftAction : public HScrollAction {

	public:
		HSLineLeftAction(TextEditor* textEditor);
		virtual ~HSLineLeftAction();

		virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

};

#endif //_HSLINELEFTACTION_H