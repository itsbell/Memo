//CtrlShiftHomeKeyAction.h
#ifndef _CTRLSHIFTHOMEKEYACTION_H
#define _CTRLSHIFTHOMEKEYACTION_H

#include "KeyAction.h"

class CtrlShiftHomeKeyAction : public KeyAction {

	public:
		CtrlShiftHomeKeyAction(TextEditor* textEditor);
		virtual ~CtrlShiftHomeKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLSHIFTHOMEKEYACTION_H