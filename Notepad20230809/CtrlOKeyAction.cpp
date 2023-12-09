//CtrlOKeyAction.cpp
#include "CtrlOKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlOKeyAction::CtrlOKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlOKeyAction::~CtrlOKeyAction() {

}

void CtrlOKeyAction::OnKeyDown(UINT nChar, UINT RepCnt, UINT nFlags) {
	this->textEditor->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_OPEN, NULL));
}