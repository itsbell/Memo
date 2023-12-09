//Scroll.cpp
#include "Scroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Scroll::Scroll() {
	this->minimum = 0;
	this->maximum = 0;
	this->page = 0;
	this->position = 0;
}

Scroll::Scroll(int minimum, int maximum, UINT page, int position) {
	this->minimum = minimum;
	this->maximum = maximum;
	this->page = page;
	this->position = position;
}

Scroll::~Scroll() {

}

int Scroll::SetPosition(int position) {
	this->position = position;

	return this->position;
}

int Scroll::SetMaximum(int maximum) {
	this->maximum = maximum;

	return this->maximum;
}