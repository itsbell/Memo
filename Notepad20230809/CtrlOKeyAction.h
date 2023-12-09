//CtrlOKeyAction.h
#ifndef _CTRLOKEYACTION_H
#define _CTRLOKEYACTION_H

#include "KeyAction.h"

class CtrlOKeyAction : public KeyAction {

	public:
		CtrlOKeyAction(TextEditor* textEditor);
		virtual ~CtrlOKeyAction();
		
		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


#endif //_CTRLOKEYACTION_H