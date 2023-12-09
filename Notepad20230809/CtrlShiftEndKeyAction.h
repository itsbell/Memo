//CtrlShiftEndKeyAction.h
#ifndef _CTRLSHIFTENDKEYACTION_H
#define _CTRLSHIFTENDKEYACTION_H

#include "KeyAction.h"

class CtrlShiftEndKeyAction : public KeyAction {

	public:
		CtrlShiftEndKeyAction(TextEditor* textEditor);
		virtual ~CtrlShiftEndKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLSHIFTENDKEYACTION_H