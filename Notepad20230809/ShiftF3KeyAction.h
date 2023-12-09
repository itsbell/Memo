//ShiftF3KeyAction.h
#ifndef _SHIFTF3KEYACTION_H
#define _SHIFTF3KEYACTION_H

#include "KeyAction.h"

class ShiftF3KeyAction : public KeyAction {

	public:
		ShiftF3KeyAction(TextEditor* textEditor);
		virtual ~ShiftF3KeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SHIFTF3KEYACTION_H