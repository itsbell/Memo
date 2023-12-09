//HorizontalScroll.h
#ifndef _HORIZONTALSCROLL_H
#define _HORIZONTALSCROLL_H

#include "Scroll.h"

class HorizontalScroll : public Scroll {
	
	public:
		HorizontalScroll();
		HorizontalScroll(int minimum, int maximum, UINT page, int position);
		virtual ~HorizontalScroll();
		
};

#endif //_HORIZONTALSCROLL_H