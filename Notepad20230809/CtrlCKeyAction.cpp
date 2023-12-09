//CtrlCKeyAction.cpp
#include "CtrlCKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CtrlCKeyAction::CtrlCKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlCKeyAction::~CtrlCKeyAction() {

}

void CtrlCKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_COPY, NULL));

}