//CtrlShiftSKeyAction.cpp
#include "CtrlShiftSKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlShiftSKeyAction::CtrlShiftSKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlShiftSKeyAction::~CtrlShiftSKeyAction() {

}

void CtrlShiftSKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_SAVEAS, NULL));
}