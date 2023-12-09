//CtrlAKeyAction.h
#ifndef _CTRLAKEYACTION_H
#define _CTRLAKEYACTION_H

#include "KeyAction.h"

class CtrlAKeyAction : public KeyAction {
	
	public:
		CtrlAKeyAction(TextEditor* textEditor);
		virtual ~CtrlAKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLAKEYACTION_H