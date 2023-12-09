//CtrlShiftSKeyAction.h
#ifndef _CTRLSHIFTSKEYACTION_H
#define _CTRLSHIFTSKEYACTION_H

#include "KeyAction.h"

class CtrlShiftSKeyAction : public KeyAction {

	public:
		CtrlShiftSKeyAction(TextEditor* textEditor);
		virtual ~CtrlShiftSKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};



#endif //_CTRLSHIFTSKEYACTION_H