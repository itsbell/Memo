//CtrlCKeyAction.h
#ifndef _CTRLCKEYACTION_H
#define _CTRLCKEYACTION_H

#include "KeyAction.h"

class CtrlCKeyAction : public KeyAction {

	public:
		CtrlCKeyAction(TextEditor* textEditor);
		virtual ~CtrlCKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLCKEYACTION_H