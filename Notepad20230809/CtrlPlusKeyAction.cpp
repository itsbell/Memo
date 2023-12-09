//CtrlPlusKeyAction.cpp
#include "CtrlPlusKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CtrlPlusKeyAction::CtrlPlusKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlPlusKeyAction::~CtrlPlusKeyAction() {

}

void CtrlPlusKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_VIEW_ZOOMIN,NULL));
}