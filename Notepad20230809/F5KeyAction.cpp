//F5KeyAction.cpp
#include "F5KeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

F5KeyAction::F5KeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

F5KeyAction::~F5KeyAction() {

}

void F5KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DATETIME, NULL));
}