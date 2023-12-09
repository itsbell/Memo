//F5KeyAction.h
#ifndef _F5KEYACTION_H
#define _F5KEYACTION_H

#include "KeyAction.h"

class F5KeyAction : public KeyAction {

	public:
		F5KeyAction(TextEditor* textEditor);
		virtual ~F5KeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_F5KEYACTION_H