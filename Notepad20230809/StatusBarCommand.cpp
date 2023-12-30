//StatusBarCommand.cpp
#include "StatusBarCommand.h"

#include "Glyph.h"
#include "Notepad.h"
#include "Registry.h"
#include "resource.h"
#include "TextEditor.h"
#include "ZoomController.h"
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
	Registry registry;
	Notepad* notepad = (Notepad*)this->textEditor->GetParent();
	CMenu* pMenu = notepad->GetMenu();

	notepad->GetClientRect(&rect);
	if (pMenu->GetMenuState(IDM_VIEW_STATUSBAR, MF_BYCOMMAND) == MF_CHECKED) {
		this->textEditor->MoveWindow(MARGIN, MARGIN, rect.Width()- MARGIN, rect.Height()- MARGIN, TRUE);
		pMenu->CheckMenuItem(IDM_VIEW_STATUSBAR, MF_UNCHECKED);
		registry.SetOnStatusBar(false);
	}

	else {
		Long magnification = 70;
		Long cRLF = 130;
		Long encoding = 120;
		Long currentPosition = rect.Width() - magnification - cRLF - encoding;
		notepad->textEditor->MoveWindow(MARGIN, MARGIN, rect.Width() - MARGIN, rect.Height() - MARGIN - STATUSBARHEIGHT, TRUE);
		notepad->sb->MoveWindow(0, rect.bottom - STATUSBARHEIGHT, rect.right, STATUSBARHEIGHT, TRUE);
		notepad->sb->MoveWindow(rect.left, rect.bottom - STATUSBARHEIGHT, rect.Width(), STATUSBARHEIGHT);
		notepad->sb->SetPaneInfo(0, IDS_CURRENT, SBPS_NORMAL, currentPosition);
		notepad->sb->SetPaneInfo(1, IDS_MAGNIFICATION, SBPS_NORMAL, magnification);
		notepad->sb->SetPaneInfo(2, IDS_CRLF, SBPS_NORMAL, cRLF);
		notepad->sb->SetPaneInfo(3, IDS_ENCODING, SBPS_NORMAL, encoding);
		pMenu->CheckMenuItem(IDM_VIEW_STATUSBAR, MF_CHECKED);
		registry.SetOnStatusBar(true);
	}
}

void StatusBarCommand::Unexecute() {

}