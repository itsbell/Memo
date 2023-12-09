//VScrollActionFactory.cpp
#include "VScrollActionFactory.h"
#include "VScrollAction.h"
#include "VSLineUpAction.h"
#include "VSLineDownAction.h"
#include "VSPageUpAction.h"
#include "VSPageDownAction.h"
#include "VSThumbTrackAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

VScrollActionFactory::VScrollActionFactory(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

VScrollActionFactory::~VScrollActionFactory() {

}

VScrollAction* VScrollActionFactory::Create(UINT nSBCode) {
	VScrollAction* vScrollAction = 0;

	if (nSBCode == SB_LINEUP) {
		vScrollAction = new VSLineUpAction(this->textEditor);
	}

	else if (nSBCode == SB_LINEDOWN) {
		vScrollAction = new VSLineDownAction(this->textEditor);
	}

	else if (nSBCode == SB_PAGEUP) {
		vScrollAction = new VSPageUpAction(this->textEditor);
	}

	else if (nSBCode == SB_PAGEDOWN) {
		vScrollAction = new VSPageDownAction(this->textEditor);
	}

	else if (nSBCode == SB_THUMBTRACK) {
		vScrollAction = new VSThumbTrackAction(this->textEditor);
	}

	return vScrollAction;
}