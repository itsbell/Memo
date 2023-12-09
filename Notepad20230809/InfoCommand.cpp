//InfoCommand.cpp
#include "InfoCommand.h"
#include "InfoForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

InfoCommand::InfoCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

InfoCommand::~InfoCommand() {

}

void InfoCommand::Execute() {
	InfoForm infoForm;

	infoForm.DoModal();
}

void InfoCommand::Unexecute() {

}