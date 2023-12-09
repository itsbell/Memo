//CtrlHomeKeyAction.h
#ifndef _CTRLHOMEKEYACTION_H
#define _CTRLHOMEKEYACTION_H

#include "KeyAction.h"

class TextEditor;

class CtrlHomeKeyAction : public KeyAction {

	public:
		CtrlHomeKeyAction();
		CtrlHomeKeyAction(TextEditor* textEditor);
		virtual ~CtrlHomeKeyAction();
	
		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLHOMEKEYACTION_H