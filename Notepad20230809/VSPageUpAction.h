//VSPageUpAction.h
#ifndef _VSPAGEUPACTION_H
#define _VSPAGEUPACTION_H

#include "VScrollAction.h"

class VSPageUpAction : public VScrollAction {
	
	public:
		VSPageUpAction(TextEditor* textEditor);
		virtual ~VSPageUpAction();

		virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#endif //_VSPAGEUPACTION_H