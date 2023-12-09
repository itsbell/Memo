//BackSpaceKeyAction.cpp
#include "BackSpaceKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BackSpaceKeyAction::BackSpaceKeyAction() {

}

BackSpaceKeyAction::BackSpaceKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

BackSpaceKeyAction::~BackSpaceKeyAction() {

}

void BackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_BACKSPACEKEY, NULL));
}