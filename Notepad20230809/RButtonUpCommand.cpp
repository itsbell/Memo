//RButtonUpCommand.cpp
#include "RButtonUpCommand.h"
#include "TextEditor.h"
#include "Document.h"
#include "Stack.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif
RButtonUpCommand::RButtonUpCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

RButtonUpCommand::~RButtonUpCommand() {

}

void RButtonUpCommand::Execute() {
	CMenu menu;
	unsigned int priority_list = CF_TEXT;

	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, IDM_EDIT_UNDO, "실행 취소");
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, IDM_EDIT_CUT, "잘라내기");
	menu.AppendMenu(MF_STRING, IDM_EDIT_COPY, "복사");
	menu.AppendMenu(MF_STRING, IDM_EDIT_PASTE, "붙여넣기");
	menu.AppendMenu(MF_STRING, IDM_EDIT_DELETE, "삭제");
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, IDM_EDIT_SELECTALL, "모두 선택");

	if (this->textEditor->unDoCommands == 0 || this->textEditor->unDoCommands->GetLength() == 0) {
		menu.EnableMenuItem(IDM_EDIT_UNDO, MF_DISABLED);
	}
	if (this->textEditor->document->isSelecting == false) {
		menu.EnableMenuItem(IDM_EDIT_CUT, MF_DISABLED);
		menu.EnableMenuItem(IDM_EDIT_COPY, MF_DISABLED);
		menu.EnableMenuItem(IDM_EDIT_DELETE, MF_DISABLED);
	}
	if (::GetPriorityClipboardFormat(&priority_list, 1) != CF_TEXT) {
		menu.EnableMenuItem(IDM_EDIT_PASTE, MF_DISABLED);
	}

	::ClientToScreen(this->textEditor->m_hWnd, &(this->textEditor->point));
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, this->textEditor->point.x, this->textEditor->point.y, this->textEditor, 0);
}

void RButtonUpCommand::Unexecute() {

}