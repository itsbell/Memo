//SelectingDeleteKeyAction.h
#ifndef _SELECTINGDELETEKEYACTION_H
#define _SELECTINGDELETEKEYACTION_H

#include "KeyAction.h"

class SelectingDeleteKeyAction : public KeyAction {
	
	public:
		SelectingDeleteKeyAction(TextEditor* textEditor);
		virtual ~SelectingDeleteKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SELECTINGDELETEKEYACTION_H