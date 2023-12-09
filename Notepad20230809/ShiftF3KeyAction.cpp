//ShiftF3KeyAction.cpp
#include "ShiftF3KeyAction.h"
#include "TextEditor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ShiftF3KeyAction::ShiftF3KeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ShiftF3KeyAction::~ShiftF3KeyAction() {

}

void ShiftF3KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_FINDPREVIOUS, NULL));
}