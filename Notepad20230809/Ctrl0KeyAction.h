//Ctrl0KeyAction.h
#ifndef _CTRL0KEYACTION_H
#define _CTRL0KEYACTION_H

#include "KeyAction.h"

class Ctrl0KeyAction : public KeyAction {

	public:
		Ctrl0KeyAction(TextEditor* textEditor);
		virtual ~Ctrl0KeyAction();
		
		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRL0KEYACTION_H