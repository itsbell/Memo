//HomeKeyAction.h
#ifndef _HOMEKEYACTION_H
#define _HOMEKEHACTION_H

#include "KeyAction.h"

class TextEditor;

class HomeKeyAction : public KeyAction {

	public: 
		HomeKeyAction();
		HomeKeyAction(TextEditor* textEditor);
		virtual ~HomeKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_HOMEKEYACTION_H