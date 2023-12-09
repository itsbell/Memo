//LeftKeyAction.h
#ifndef _LEFTKEYACTION_H
#define _LEFTKEYACTION_H

#define ASCII 128

#include "KeyAction.h"

class TextEditor;

class LeftKeyAction : public KeyAction {

	public:
		LeftKeyAction();
		LeftKeyAction(TextEditor* textEditor);
		virtual ~LeftKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_LEFTKEYACTION_H