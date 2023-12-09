//CtrlWKeyAction.h
#ifndef _CTRLWKEYACTION_H
#define _CTRLWKEYACTION_H

#include "KeyAction.h"

class CtrlWKeyAction : public KeyAction {
	
	public:
		CtrlWKeyAction(TextEditor* textEditor);
		virtual ~CtrlWKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLWKEYACTION_H