//SelectingRightKeyAction.h
#ifndef _SELECTINGRIGHTKEYACTION_H
#define _SELECTINGRIGHTKEYACTION_H

#include "KeyAction.h"

class SelectingRightKeyAction : public KeyAction {

	public:
		SelectingRightKeyAction(TextEditor* textEditor);
		virtual ~SelectingRightKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SELECTINGRIGHTKEYACTION_H