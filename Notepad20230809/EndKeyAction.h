//EndKeyAction.h
#ifndef _ENDKEYACTION_H
#define _ENDKEYACTION_H

#include "KeyAction.h"

class TextEditor;
class EndKeyAction : public KeyAction {

	public: 
		EndKeyAction();
		EndKeyAction(TextEditor* textEditor);
		virtual ~EndKeyAction();
		
		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_ENDKEYACTION_H