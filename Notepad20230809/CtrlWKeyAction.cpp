//CtrlWKeyAction.cpp
#include "CtrlWKeyAction.h"
#include "TextEditor.h"


CtrlWKeyAction::CtrlWKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlWKeyAction::~CtrlWKeyAction() {

}

void CtrlWKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->GetParent()->SendMessage(WM_CLOSE, NULL);
}