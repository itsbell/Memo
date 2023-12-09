//CtrlSKeyAction.cpp
#include "CtrlSKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlSKeyAction::CtrlSKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlSKeyAction::~CtrlSKeyAction() {

}

void CtrlSKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_SAVE, NULL));
}