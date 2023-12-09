//NotepadCommandFactory.cpp
#include "NotepadCommandFactory.h"
#include "NewCommand.h"
#include "OpenCommand.h"
#include "SaveCommand.h"
#include "SaveAsCommand.h"
#include "PageSetupCommand.h"
#include "PreviewCommand.h"
#include "PrintCommand.h"
#include "StatusBarCommand.h"
#include "ShowHelpCommand.h"
#include "InfoCommand.h"
#include "ExitCommand.h"
#include "CloseCommand.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

NotepadCommandFactory::NotepadCommandFactory(Notepad* notepad) {
	this->notepad = notepad;
}

NotepadCommandFactory::~NotepadCommandFactory() {

}

Command* NotepadCommandFactory::Create(UINT nID) {

	Command* command = 0;

	if (nID == IDM_FILE_NEW) {
		command = new NewCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_FILE_OPEN) {
		command = new OpenCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_FILE_SAVE) {
		command = new SaveCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_FILE_SAVEAS) {
		command = new SaveAsCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_FILE_PAGESETUP) {
		command = new PageSetupCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_FILE_PREVIEW) {
		command = new PreviewCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_FILE_PRINT) {
		command = new PrintCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_FILE_EXIT) {
		command = new ExitCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_FILE_CLOSE) {
		command = new CloseCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_VIEW_STATUSBAR) {
		command = new StatusBarCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_HELP_HELP) {
		command = new ShowHelpCommand(this->notepad->textEditor);
	}

	else if (nID == IDM_HELP_INFO) {
		command = new InfoCommand(this->notepad->textEditor);
	}

	return command;
}