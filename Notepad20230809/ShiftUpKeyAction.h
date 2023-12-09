//ShiftUpKeyAction.h
#ifndef _SHIFTUPKEYACTION_H
#define _SHIFTUPKEYACTION_H

#include "KeyAction.h"

class ShiftUpKeyAction : public KeyAction {

	public:
		ShiftUpKeyAction(TextEditor* textEditor);
		virtual ~ShiftUpKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SHIFTUPKEYACTION_H