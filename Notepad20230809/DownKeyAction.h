//DownKeyAction.h
#ifndef _DOWNKEYACTION_H
#define _DOWNKEYACTION_H

#include "KeyAction.h"

typedef signed long int Long;

class TextEditor;

class DownKeyAction : public KeyAction {

	public:
		DownKeyAction();
		DownKeyAction(TextEditor* textEditor);
		virtual ~DownKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_DOWNKEYACTION_H