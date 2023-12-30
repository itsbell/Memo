//CtrlWKeyAction.cpp
#include "CtrlWKeyAction.h"
#include "TextEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlWKeyAction::CtrlWKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlWKeyAction::~CtrlWKeyAction() {

}

void CtrlWKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->GetParent()->SendMessage(WM_CLOSE, NULL);
}