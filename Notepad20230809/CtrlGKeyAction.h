//CtrlGKeyAction.h
#ifndef _CTRLGKEYACTION_H
#define _CTRLGKEYACTION_H

#include "KeyAction.h"

class CtrlGKeyAction : public KeyAction {

	public:
		CtrlGKeyAction(TextEditor* textEditor);
		virtual ~CtrlGKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLGKEYACTION_H