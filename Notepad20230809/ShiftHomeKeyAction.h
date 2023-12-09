//ShiftHomeKeyAction.h
#ifndef _SHIFTHOMEKEYACTION_H
#define _SHIFTHOMEKEYACTION_H

#include "KeyAction.h"

class ShiftHomeKeyAction : public KeyAction {

	public:
		ShiftHomeKeyAction(TextEditor* textEditor);
		virtual ~ShiftHomeKeyAction();

		virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_SHIFTHOMEKEYACTION_H