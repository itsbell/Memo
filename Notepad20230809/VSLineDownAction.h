//VSLineDownAction.h
#ifndef _VSLINEDOWNACTION_H
#define _VSLINEDOWNACTION_H

#include "VScrollAction.h"

class VSLineDownAction :public VScrollAction {

	public:
		VSLineDownAction(TextEditor* textEditor);
		virtual ~VSLineDownAction();

		virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#endif //_VSLINEDOWNACTION_H