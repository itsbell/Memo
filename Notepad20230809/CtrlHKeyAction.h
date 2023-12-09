//CtrlHKeyAction.h
#ifndef _CTRLHKEYACTION_H
#define _CTRLHKEYACTION_H

#include "KeyAction.h"

class CtrlHKeyAction : public KeyAction {

	public:
		CtrlHKeyAction(TextEditor* textEditor);
		virtual ~CtrlHKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLHKEYACTION_H