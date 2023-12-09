//CtrlPlusKeyAction.h
#ifndef _CTRLPLUSKEYACTION_H
#define _CTRLPLUSKEYACTION_H

#include "KeyAction.h"

class CtrlPlusKeyAction : public KeyAction {

	public:
		CtrlPlusKeyAction(TextEditor* textEditor);
		virtual ~CtrlPlusKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

};

#endif //_CTRLPLUSKEYACTION_H