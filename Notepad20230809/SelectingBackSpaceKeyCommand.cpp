//SelectingBackSpaceKeyCommand.cpp
#include "SelectingBackSpaceKeyCommand.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SelectingBackSpaceKeyCommand::SelectingBackSpaceKeyCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

SelectingBackSpaceKeyCommand::~SelectingBackSpaceKeyCommand() {

}

void SelectingBackSpaceKeyCommand::Execute() {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, NULL));

}

void SelectingBackSpaceKeyCommand::Unexecute() {

}