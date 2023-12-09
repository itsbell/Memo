//CtrlVKeyAction.cpp
#include "CtrlVKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlVKeyAction::CtrlVKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlVKeyAction::~CtrlVKeyAction() {

}

void CtrlVKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_PASTE, NULL));
}