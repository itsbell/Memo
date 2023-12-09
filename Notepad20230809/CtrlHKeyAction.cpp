//CtrlHKeyAction.cpp
#include "CtrlHKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlHKeyAction::CtrlHKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlHKeyAction::~CtrlHKeyAction() {

}

void CtrlHKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {

	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_REPLACE, NULL));

}