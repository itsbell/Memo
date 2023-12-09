//CtrlXKeyAction.h
#include "CtrlXKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlXKeyAction::CtrlXKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlXKeyAction::~CtrlXKeyAction() {

}

void CtrlXKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_CUT, NULL));
}