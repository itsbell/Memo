//ShowHelpCommand.cpp
#include "ShowHelpCommand.h"
#include "TextEditor.h"
#include "Notepad.h"
#include <shlobj_core.h>

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ShowHelpCommand::ShowHelpCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ShowHelpCommand::~ShowHelpCommand() {

}

void ShowHelpCommand::Execute() {
	char path[128];

	::SHGetFolderPath(NULL, CSIDL_PROGRAM_FILESX86, NULL, 0, path);
	strcat(path, "\\Notepad\\NotepadHelp.chm");
	::ShellExecute(::GetDesktopWindow(), "open", path, 0, 0, SW_SHOWDEFAULT);
}

void ShowHelpCommand::Unexecute() {

}