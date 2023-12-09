//PageDownKeyAction.h
#ifndef _PAGEDOWNKEYACTION_H
#define _PAGEDOWNKEYACTION_H

#include "KeyAction.h"

class TextEditor;

class PageDownKeyAction : public KeyAction {

	public:
		PageDownKeyAction(TextEditor* textEditor);
		virtual ~PageDownKeyAction();

		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_PAGEDOWNKEYACTION_H