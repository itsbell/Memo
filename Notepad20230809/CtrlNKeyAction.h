//CtrlNKeyAction.h
#ifndef _CTRLNKEYACTION_H
#define _CTRLNKEYACTION_H

#include "KeyAction.h"

class TextEditor;
class CtrlNKeyAction : public KeyAction {

	public:
		CtrlNKeyAction(TextEditor* textEditor);
		virtual ~CtrlNKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_CTRLNKEYACTION_H
