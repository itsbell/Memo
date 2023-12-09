//HSThumbTrackAction.h
#ifndef _HSTHUMBTRACKACTION_H
#define _HSTHUMBTRACKACTION_H

#include "HScrollAction.h"

class HSThumbTrackAction : public HScrollAction {
	
	public:
		HSThumbTrackAction(TextEditor* textEditor);
		virtual ~HSThumbTrackAction();
	
		virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#endif //_HSTHUMBTRACKACTION_H