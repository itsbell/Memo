//SelectingBackSpaceKeyAction.cpp
#include "SelectingBackSpaceKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SelectingBackSpaceKeyAction::SelectingBackSpaceKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

SelectingBackSpaceKeyAction::~SelectingBackSpaceKeyAction() {

}

void SelectingBackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {	
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, NULL));
}