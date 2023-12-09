//F11KeyAction.cpp
#include "F11KeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

F11KeyAction::F11KeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

F11KeyAction::~F11KeyAction() {

}

void F11KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_PREVIEW, NULL));
}