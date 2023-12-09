//SelectingDeleteKeyAction.cpp
#include "SelectingDeleteKeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SelectingDeleteKeyAction::SelectingDeleteKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

SelectingDeleteKeyAction::~SelectingDeleteKeyAction() {

}

void SelectingDeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, NULL));
}