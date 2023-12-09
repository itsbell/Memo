//VScrollAction.h
#ifndef _VSCROLLACTION_H
#define _VSCROLLACTION_H

#include "ScrollAction.h"

class VScrollAction : public ScrollAction{

	public:
		VScrollAction();
		virtual ~VScrollAction() = 0;

		virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) = 0;
};

#endif //_VSCROLLACTION_H