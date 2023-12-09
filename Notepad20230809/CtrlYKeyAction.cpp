//CtrlYKeyAction.cpp
#include "CtrlYKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlYKeyAction::CtrlYKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlYKeyAction::~CtrlYKeyAction() {

}

void CtrlYKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_REDO, NULL));
}