//DeleteKeyAction.h
#ifndef _DELETEKEYACTION_H
#define _DELETEKEYACTION_H

#include "KeyAction.h"

class TextEditor;

class DeleteKeyAction : public KeyAction {

	public:
		DeleteKeyAction();
		DeleteKeyAction(TextEditor* textEditor);
		virtual ~DeleteKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_DELETEKEYACTION_H