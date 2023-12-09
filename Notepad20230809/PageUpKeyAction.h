//PageUpKeyAction.h
#ifndef _PAGEUPKEYACTION_H
#define _PAGEUPKEYACTION_H

#include "KeyAction.h"

class TextEditor;

class PageUpKeyAction : public KeyAction {

	public:
		PageUpKeyAction(TextEditor* textEditor);
		virtual ~PageUpKeyAction();

		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_PAGEUPKEYACTION_H