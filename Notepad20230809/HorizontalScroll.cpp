//HorizontalScroll.cpp
#include "HorizontalScroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HorizontalScroll::HorizontalScroll() {

}

HorizontalScroll::HorizontalScroll(int minimum, int maximum, UINT page, int position)
	:Scroll(minimum,maximum,page,position){

}

HorizontalScroll::~HorizontalScroll() {

}
