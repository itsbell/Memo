//CtrlDKeyAction.h
#ifndef _CTRLDKeyAction_H
#define _CTRLDKeyAction_H

#include "KeyAction.h"

class CtrlDKeyAction : public KeyAction {
	
	public:
		CtrlDKeyAction(TextEditor* textEditor);
		virtual ~CtrlDKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_F12KeyAction_H