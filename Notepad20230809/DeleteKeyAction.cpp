//DeleteKeyAction.cpp
#include "DeleteKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DeleteKeyAction::DeleteKeyAction() {

}

DeleteKeyAction::DeleteKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

DeleteKeyAction::~DeleteKeyAction() {

}

void DeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_DELETEKEY, NULL));
}