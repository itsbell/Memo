//SelectingLeftKeyAction.h
#ifndef _SELECTINGLEFTKEYACTION_H
#define _SELECTINGLEFTKEYACTION_H

#include "KeyAction.h"

class SelectingLeftKeyAction : public KeyAction {
	
	public:
		SelectingLeftKeyAction(TextEditor* textEditor);
		virtual ~SelectingLeftKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SELECTINGLEFTKEYACTION_H