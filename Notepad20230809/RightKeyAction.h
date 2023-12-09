//RightKeyAction.h
#ifndef _RIGHTKEYACTION_H
#define _RIGHTKEYACTION_H

#include "KeyAction.h"

class TextEditor;

class RightKeyAction : public KeyAction {

	public:
		RightKeyAction();
		RightKeyAction(TextEditor* textEditor);
		virtual ~RightKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_RIGHTKEYACTION_H