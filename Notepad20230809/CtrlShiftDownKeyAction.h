//CtrlShiftDownKeyAction.h
#ifndef _CTRLSHIFTDOWNKEYACTION_H
#define _CTRLSHIFTDOWNEKYACTION_H

#include "KeyAction.h"

class CtrlShiftDownKeyAction : public KeyAction {

	public:
		CtrlShiftDownKeyAction(TextEditor* textEditor);
		virtual ~CtrlShiftDownKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLSHIFTDOWNKEYACTION_H