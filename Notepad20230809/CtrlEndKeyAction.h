//CtrlEndKeyAction.h
#ifndef _CTRLENDKEYACTION_H
#define _CTRLENDKEYACTION_H

#include "KeyAction.h"

class TextEditor;

class CtrlEndKeyAction : public KeyAction {
	
	public:
		CtrlEndKeyAction();
		CtrlEndKeyAction(TextEditor* textEditor);
		virtual ~CtrlEndKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLENDKEYACTION_H