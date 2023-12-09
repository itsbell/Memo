//CtrlShiftLeftKeyAction.h
#ifndef _CTRLSHIFTLEFTKEYACTION_H
#define _CTRLSHIFTLEFTKEYACTION_H

#include "KeyAction.h"

class CtrlShiftLeftKeyAction : public KeyAction {
	
public:
		CtrlShiftLeftKeyAction(TextEditor* textEditor);
		virtual ~CtrlShiftLeftKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLSHIFTLEFTKEYACTION_H