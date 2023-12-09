//BackSpaceKeyAction.h
#ifndef _BACKSPACEKEYACTION_H
#define _BACKSPACEKEYACTION_H

#include "KeyAction.h"

class TextEditor;

class BackSpaceKeyAction : public KeyAction {

	public: 
		BackSpaceKeyAction();
		BackSpaceKeyAction(TextEditor* textEditor);
		virtual ~BackSpaceKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_BACKSPACEKEYACTION_H