//ExitCommand.h
#include "ExitCommand.h"
#include "TextEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ExitCommand::ExitCommand(TextEditor* textEditor)
{
	this->textEditor = textEditor;
}

ExitCommand::~ExitCommand()
{
}

void ExitCommand::Execute()
{
	this->textEditor->GetParent()->SendMessage(WM_CLOSE);
}

void ExitCommand::Unexecute()
{
}
