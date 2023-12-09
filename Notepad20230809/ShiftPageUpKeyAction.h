//ShiftPageUpKeyAction.h
#ifndef _SHIFTPAGEUPKEYACTION_H
#define _SHIFTPAGEUPKEYACTION_H

#include "KeyAction.h"

class ShiftPageUpKeyAction : public KeyAction {

	public:
		ShiftPageUpKeyAction(TextEditor* textEditor);
		virtual ~ShiftPageUpKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SHIFTPAGEUPKEYACTION_H