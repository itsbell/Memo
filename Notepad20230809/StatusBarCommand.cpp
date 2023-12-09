//StatusBarCommand.cpp
#include "StatusBarCommand.h"
#include "Notepad.h"
#include "TextEditor.h"
#include "ZoomController.h"
#include "Glyph.h"
#include "resource.h"

#include <afxext.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

StatusBarCommand::StatusBarCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

StatusBarCommand::~StatusBarCommand() {

}

void StatusBarCommand::Execute() {
	CRect rect;
	this->textEditor->GetParent()->GetClientRect(&rect);

	CMenu* pMenu = this->textEditor->GetParent()->GetMenu();
	Notepad* notepad = (Notepad*)this->textEditor->GetParent();

	if (pMenu->GetMenuState(IDM_VIEW_STATUSBAR, MF_BYCOMMAND) == MF_CHECKED) {

		this->textEditor->MoveWindow(0, 0, rect.Width(), rect.Height(), TRUE);
		pMenu->CheckMenuItem(IDM_VIEW_STATUSBAR, MF_UNCHECKED);
	}

	else {
		notepad->textEditor->MoveWindow(0, 0, rect.Width(), rect.Height() - 30, TRUE);
		notepad->sb->MoveWindow(0, rect.bottom - 30, rect.right, 30, TRUE);
		pMenu->CheckMenuItem(IDM_VIEW_STATUSBAR, MF_CHECKED);
	}
}

void StatusBarCommand::Unexecute() {

}