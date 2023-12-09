//CtrlUpKeyAction.h
#ifndef _CTRLUPKEYACTION_H
#define _CTRLUPKEYACTION_H

#include "KeyAction.h"

class TextEditor;

class CtrlUpKeyAction : public KeyAction {

	public: 
		CtrlUpKeyAction();
		CtrlUpKeyAction(TextEditor* textEditor);
		virtual ~CtrlUpKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLUPKEYACTION_H