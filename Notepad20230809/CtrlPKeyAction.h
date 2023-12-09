//CtrlPKeyAction.h
#ifndef _CTRLPKEYACTION_H
#define _CTRLPKEYACTION_H

#include "KeyAction.h"

class CtrlPKeyAction : public KeyAction {

	public:
		CtrlPKeyAction(TextEditor* textEditor);
		virtual ~CtrlPKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLPKEYACTION_H