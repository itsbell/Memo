//F3KeyAction.h
#ifndef _F3KEYACTION_H
#define _F3KEYACTION_H

#include "KeyAction.h"

class F3KeyAction : public KeyAction {

	public:
		F3KeyAction(TextEditor* textEditor);
		virtual ~F3KeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_F3KEYACTION_H