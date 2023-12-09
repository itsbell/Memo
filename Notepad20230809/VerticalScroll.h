//VerticalScroll.h
#ifndef _VERTICALSCROLL_H
#define _VERTICALSCROLL_H

#include "Scroll.h"

class VerticalScroll : public Scroll {

	public:
		VerticalScroll();
		VerticalScroll(int minimum, int maximum, UINT page, int position);
		virtual ~VerticalScroll();

};

#endif //_VERTICALSCROLL_H