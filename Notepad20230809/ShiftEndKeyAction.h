//ShiftEndKeyAction.h
#ifndef _SHIFTENDKEYACTION_H
#define _SHIFTENDKEYACTION_H

#include "KeyAction.h"

class ShiftEndKeyAction : public KeyAction {

	public:
		ShiftEndKeyAction(TextEditor* textEditor);
		virtual ~ShiftEndKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SHIFTENDKEYACTION_H