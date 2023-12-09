//CtrlFKeyAction.cpp
#include "CtrlFKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlFKeyAction::CtrlFKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlFKeyAction::~CtrlFKeyAction() {

}

void CtrlFKeyAction::OnKeyDown(UINT NChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_FIND, NULL));
}