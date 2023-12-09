//CtrlMinusKeyAction.cpp
#include "CtrlMinusKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlMinusKeyAction::CtrlMinusKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlMinusKeyAction::~CtrlMinusKeyAction() {

}

void CtrlMinusKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_VIEW_ZOOMOUT, NULL));
}