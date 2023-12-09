//CtrlFKeyAction.h
#ifndef _CTRLFKEYACTION_H
#define _CTRLFKEYACTION_H

#include "KeyAction.h"

class CtrlFKeyAction : public KeyAction {

	public:
		CtrlFKeyAction(TextEditor* textEditor);
		virtual ~CtrlFKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLFKEYACTION_H