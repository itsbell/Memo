//SelectingBackSpaceKeyAction.h
#ifndef _SELECTINGBACKSPACEKEYACTION_H
#define _SELECTINGBACKSPACEKEYACTION_H

#include "KeyAction.h"

class SelectingBackSpaceKeyAction : public KeyAction {

	public:
		SelectingBackSpaceKeyAction(TextEditor* textEditor);
		virtual ~SelectingBackSpaceKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SELECTINGBACKSPACEKEYACTION_H