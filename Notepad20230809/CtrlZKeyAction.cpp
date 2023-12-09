//CtrlZKeyAction.cpp
#include "CtrlZKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlZKeyAction::CtrlZKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlZKeyAction::~CtrlZKeyAction() {

}

void CtrlZKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_UNDO, NULL));
}