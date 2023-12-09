//CtrlMinusKeyAction.h
#ifndef _CTRLMINUSKEYACTION_H
#define _CTRLMINUSKEYACTION_H

#include "KeyAction.h"

class CtrlMinusKeyAction : public KeyAction {

	public:
		CtrlMinusKeyAction(TextEditor* textEditor);
		virtual ~CtrlMinusKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLMINUSKEYACTION_H