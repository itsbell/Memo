//ZoomController.cpp
#include "ZoomController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ZoomController::ZoomController() {
	this->magnification = 1.0F;
}

ZoomController::~ZoomController() {

}

void ZoomController::ZoomIn() {
	
	this->magnification += 0.1F;
	
	if (this->magnification > 4.99F) {
		this->magnification = 5.0F;
	}
}

void ZoomController::ZoomOut() {

	this->magnification -= 0.1F;

	if (this->magnification < 0.1F) {
		this->magnification = 0.1F;
	}
}

void ZoomController::ZoomSet(float magnification) {
	this->magnification = magnification;
}

void ZoomController::Reset() {
	this->magnification = 1.0F;
}