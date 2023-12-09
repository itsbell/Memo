//F3KeyAction.cpp
#include "F3KeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

F3KeyAction::F3KeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

F3KeyAction::~F3KeyAction() {

}

void F3KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_FINDNEXT, NULL));
}