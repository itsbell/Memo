//CtrlLeftKeyAction.h
#ifndef _CTRLLEFTKEYACTION_H
#define _CTRLLEFTKEYACTION_H

#include "KeyAction.h"

class TextEditor;

class CtrlLeftKeyAction : public KeyAction {

	public:
		CtrlLeftKeyAction();
		CtrlLeftKeyAction(TextEditor* textEditor);
		virtual ~CtrlLeftKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLLEFTKEYACTION_H