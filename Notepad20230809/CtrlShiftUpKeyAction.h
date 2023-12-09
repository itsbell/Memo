//CtrlShiftUpKeyAction.h
#ifndef _CTRLSHIFTUPKEYACTION_H
#define _CTRLSHIFTUPKEYACTION_H

#include "KeyAction.h"

class CtrlShiftUpKeyAction : public KeyAction {

	public:
		CtrlShiftUpKeyAction(TextEditor* textEditor);
		virtual ~CtrlShiftUpKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLSHIFTUPKEYACTION_H