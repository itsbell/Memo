//HScrollActionFactory.cpp
#include "HScrollActionFactory.h"
#include "HScrollAction.h"
#include "HSLineLeftAction.h"
#include "HSLineRightAction.h"
#include "HSPageLeftAction.h"
#include "HSPageRightAction.h"
#include "HSThumbTrackAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


HScrollActionFactory::HScrollActionFactory(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

HScrollActionFactory::~HScrollActionFactory() {

}

HScrollAction* HScrollActionFactory::Create(UINT nSBCode) {
	HScrollAction* hScrollAction = 0;

	if (nSBCode == SB_LINELEFT) {
		hScrollAction = new HSLineLeftAction(this->textEditor);
	}

	else if (nSBCode == SB_LINERIGHT) {
		hScrollAction = new HSLineRightAction(this->textEditor);
	}

	else if (nSBCode == SB_PAGELEFT) {
		hScrollAction = new HSPageLeftAction(this->textEditor);
	}

	else if (nSBCode == SB_PAGERIGHT) {
		hScrollAction = new HSPageRightAction(this->textEditor);
	}

	else if (nSBCode == SB_THUMBTRACK) {
		hScrollAction = new HSThumbTrackAction(this->textEditor);
	}

	return hScrollAction;
}