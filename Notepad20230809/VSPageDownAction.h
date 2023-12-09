//VSPageDownAction.h
#ifndef _VSPAGEDOWNACTION_H
#define _VSPAGEDOWNACTION_H

#include "VScrollAction.h"

class VSPageDownAction : public VScrollAction {

	public:
		VSPageDownAction(TextEditor* textEditor);
		virtual ~VSPageDownAction();

		virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#endif //_VSPAGEDOWNACTION_H