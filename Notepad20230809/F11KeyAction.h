//F11KeyAction
#ifndef _F11KEYACTION_H
#define _F11KEYACTION_H

#include "KeyAction.h"

class F11KeyAction : public KeyAction {
	
	public:
		F11KeyAction(TextEditor* textEditor);
		virtual ~F11KeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_F11KeyAction_H