//CtrlYKeyAction.h
#ifndef _CTRLYKEYACTION_H
#define _CTRLYKEYACTION_H

#include "KeyAction.h"

class CtrlYKeyAction : public KeyAction {

	public:
		CtrlYKeyAction(TextEditor* textEditor);
		virtual ~CtrlYKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLYKEYACTION_H