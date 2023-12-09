//ShiftLeftKeyAction.h
#ifndef _SHIFTLEFTKEYACTION_H
#define _SHIFTLEFTKEYACTION_H

#include "KeyAction.h"

class ShiftLeftKeyAction : public KeyAction {

	public:
		ShiftLeftKeyAction(TextEditor* textEditor);
		~ShiftLeftKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SHIFTLEFTKEYACTION_H