//VerticalScroll.cpp
#include "VerticalScroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

VerticalScroll::VerticalScroll() {

}

VerticalScroll::VerticalScroll(int minimum, int maximum, UINT page, int position)
	:Scroll(minimum, maximum, page, position) {

}

VerticalScroll::~VerticalScroll() {

}
