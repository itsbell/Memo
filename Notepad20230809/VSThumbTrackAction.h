//VSThumbTrackAction.h
#ifndef _VSTHUMBTRACKACTION_H
#define _VSTHUMBTRACKACTION_H

#include "VScrollAction.h"

class VSThumbTrackAction : public VScrollAction {

	public: 
		VSThumbTrackAction(TextEditor* textEditor);
		virtual ~VSThumbTrackAction();

		virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#endif //_VSTHUMBTRACKACTION_H