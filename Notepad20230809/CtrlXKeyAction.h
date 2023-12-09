//CtrlXKeyAction.h
#ifndef _CTRLXKEYACTION_H
#define _CTRLXKEYACTION_H

#include "KeyAction.h"

class CtrlXKeyAction : public KeyAction {

	public:
		CtrlXKeyAction(TextEditor* textEditor);
		virtual ~CtrlXKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLXKEYACTION_H