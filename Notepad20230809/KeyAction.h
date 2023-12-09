//KeyAction.h

#ifndef _KEYACTION_H
#define _KEYACTION_H

#include <afxwin.h>

class TextEditor;

class KeyAction {

	public:
		KeyAction();
		virtual ~KeyAction() = 0;

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;

	protected:
		TextEditor* textEditor;
};

#endif //_KEYACTION_H