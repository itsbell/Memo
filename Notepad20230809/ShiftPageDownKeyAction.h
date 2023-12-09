//ShiftPageDownKeyAction.h
#ifndef _SHIFTPAGEDOWNKEYACTION_H
#define _SHIFTPAGEDOWNKEYACTION_H

#include "KeyAction.h"

class ShiftPageDownKeyAction : public KeyAction {

	public:
		ShiftPageDownKeyAction(TextEditor* textEditor);
		virtual ~ShiftPageDownKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SHIFTPAGEDOWNKEYACTION_H