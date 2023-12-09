//CtrlDownKeyAction.h
#ifndef _CTRLDOWNKEYACTION_H
#define _CTRLDOWNKEYACTION_H

#include "KeyAction.h"

class TextEditor;

class CtrlDownKeyAction : public KeyAction {

	public:
		CtrlDownKeyAction();
		CtrlDownKeyAction(TextEditor* textEditor);
		virtual ~CtrlDownKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLDOWNKEYACTION_H