//CtrlShiftRightKeyAction.h
#ifndef _CTRLSHIFTRIGHTKEYACTION_H
#define _CTRLSHIFTRIGHTKEYACTION_H

#include "KeyAction.h"

class CtrlShiftRightKeyAction : public KeyAction {
	
	public:
		CtrlShiftRightKeyAction(TextEditor* textEditor);
		virtual ~CtrlShiftRightKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLSHIFTRIGHTKEYACTION_H