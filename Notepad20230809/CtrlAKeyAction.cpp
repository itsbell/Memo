//CtrlAKeyAction.cpp
#include "CtrlAKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlAKeyAction::CtrlAKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlAKeyAction::~CtrlAKeyAction() {

}

void CtrlAKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_SELECTALL, NULL));
}