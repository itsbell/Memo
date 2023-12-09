//ShiftDownKeyAction.h
#ifndef _SHIFTDOWNKEYACTION_H
#define _SHIFTDOWNKEYACTION_H

#include "KeyAction.h"

class ShiftDownKeyAction : public KeyAction {
	
	public:
		ShiftDownKeyAction(TextEditor* textEditor);
		virtual ~ShiftDownKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SHIFTDOWNKEYACTION_H