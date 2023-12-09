//VSLineUpAction.h
#ifndef _VSLINEUPACTION_H
#define _VSLINEUPACTION_H

#include "VScrollAction.h"

class VSLineUpAction : public VScrollAction {
	
	public:
		VSLineUpAction(TextEditor* textEditor);
		virtual ~VSLineUpAction();

		virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#endif //_VSLINEUPACTION_H