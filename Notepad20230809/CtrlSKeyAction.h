//CtrlSKeyAction.h
#ifndef _CTRLSKEYACTION_H
#define _CTRLSKEYACTION_H

#include "KeyAction.h"

class CtrlSKeyAction : public KeyAction {

	public:
		CtrlSKeyAction(TextEditor* textEditor);
		virtual ~CtrlSKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLSKEYACTION_H