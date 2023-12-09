//Ctrl0KeyAction.cpp
#include "Ctrl0KeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Ctrl0KeyAction::Ctrl0KeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

Ctrl0KeyAction::~Ctrl0KeyAction() {

}

void Ctrl0KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_VIEW_RESET, NULL));
}