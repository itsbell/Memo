//CtrlVKeyAction.h
#ifndef _CTRLVKEYACTION_H
#define _CTRLVKEYACTION_H

#include "KeyAction.h"

class CtrlVKeyAction : public KeyAction {

	public:
		CtrlVKeyAction(TextEditor* textEditor);
		virtual ~CtrlVKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLVKEYACTION_H