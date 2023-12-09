//CtrlGKeyAction.cpp
#include "CtrlGKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlGKeyAction::CtrlGKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlGKeyAction::~CtrlGKeyAction() {

}

void CtrlGKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_MOVE, NULL));
}