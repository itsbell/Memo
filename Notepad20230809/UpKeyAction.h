//UpKeyAction.h
#ifndef _UPKEYACTION_H
#define _UPKEYACTION_H

#include "KeyAction.h"

class TextEditor;
class UpKeyAction : public KeyAction {

	public:
		UpKeyAction();
		UpKeyAction(TextEditor* textEditor);
		virtual ~UpKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_UPKEYACTION_H