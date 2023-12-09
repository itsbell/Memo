//HScrollAction.h
#ifndef _HSCROLLACTION_H
#define _HSCROLLACTION_H

#include "ScrollAction.h"

class HScrollAction : public ScrollAction{

	public:
		HScrollAction();
		virtual ~HScrollAction() = 0;

		virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) = 0;

};

#endif //_HSCROLLACTION_H