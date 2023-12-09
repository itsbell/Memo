//CtrlZKeyAction.h
#ifndef _CTRLZKEYACTION_H
#define _CTRLZKEYACTION_H

#include "KeyAction.h"

class CtrlZKeyAction : public KeyAction {

	public:
		CtrlZKeyAction(TextEditor* textEditor);
		virtual ~CtrlZKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLZKEYACTION_H