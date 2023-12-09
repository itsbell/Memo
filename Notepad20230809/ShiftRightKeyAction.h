//ShiftRightKeyAction.h
#ifndef _SHIFTRIGHTKEYACTION_H
#define _SHIFTRIGHTKEYACTION_H

#include "KeyAction.h"

class ShiftRightKeyAction : public KeyAction {
	
	public:
		ShiftRightKeyAction(TextEditor* textEditor);
		virtual ~ShiftRightKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SHIFTRIGHTKEYACTION_H