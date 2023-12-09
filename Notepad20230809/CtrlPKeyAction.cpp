//CtrlPKeyAction.cpp
#include "CtrlPKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlPKeyAction::CtrlPKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlPKeyAction::~CtrlPKeyAction() {

}

void CtrlPKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_PRINT, NULL));
}