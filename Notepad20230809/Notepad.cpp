//Notepad.cpp
#include "Notepad.h"

#include "TextEditor.h"
#include "resource.h"
#include "Command.h"
#include "NotepadCommandFactory.h"
#include "Glyph.h"
#include "Stack.h"
#include "Document.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable:4996)

#include <afxext.h>

static UINT indicators[] = { IDS_CURRENT, IDS_MAGNIFICATION, IDS_CRLF, IDS_ENCODING };

BEGIN_MESSAGE_MAP(Notepad, CFrameWnd)

	ON_WM_CREATE()
	ON_COMMAND_RANGE(IDM_FILE_NEW, IDM_FILE_CLOSE, OnMenuClicked)
	ON_COMMAND_RANGE(IDM_EDIT_UNDO, IDM_VIEW_WRAP, OnTextEditorMenuClicked)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_REDO, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_UNDO, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_CUT, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_COPY, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_DELETE, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_PASTE, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_FIND, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_FINDNEXT, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_FINDPREVIOUS, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_REPLACE, OnUpdateMenu)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()

END_MESSAGE_MAP()


Notepad::Notepad() {
	this->textEditor = 0;
	this->sb = 0;
	this->exit = false;
}

Notepad::~Notepad() {

	if (this->textEditor != 0) {
		delete this->textEditor;
	}
	if (this->sb != 0) {
		delete this->sb;
	}
}

int Notepad::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_MEMO));
	this->SetIcon(hIcon, TRUE);

	CRect client;
	this->GetClientRect(&client);

	CRect textEditorRect(client.left + MARGIN, client.top - MARGIN, client.right - MARGIN, client.bottom - 30 - MARGIN);

	this->textEditor = new TextEditor;
	this->textEditor->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, textEditorRect, this, IDC_TEXTEDITOR);

	//상태표시줄
	this->sb = new CStatusBar;
	this->sb->Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, AFX_IDW_STATUS_BAR);
	this->sb->SetIndicators(indicators, 4);
	this->menu.LoadMenuA(IDR_MENU1);
	this->SetMenu(&this->menu);
	if (this->textEditor->isWrapped) {
		this->menu.CheckMenuItem(IDM_VIEW_WRAP, MF_CHECKED);
	}
	this->textEditor->SetFocus();

	return 0;
}

void Notepad::OnSetFocus(CWnd* pOldWnd) {
	this->textEditor->SetFocus();
}

void Notepad::OnSize(UINT nType, int cx, int cy) {
	
	if (cx > 0 ) { // 최소화 시 SetPaneInfo가 버그를 발생 시키기 때문에 넣은 if문입니다.
		CRect rect;
		this->GetClientRect(&rect);

		if (this->GetMenu()->GetMenuState(IDM_VIEW_STATUSBAR, MF_BYCOMMAND) == MF_CHECKED) {
			this->textEditor->MoveWindow(MARGIN, MARGIN, rect.Width() - MARGIN, rect.Height() - 30 - MARGIN, FALSE);
		
			Long magnification = 70;
			Long cRLF = 130;
			Long encoding = 120;
			Long currentPosition = rect.Width() - magnification - cRLF - encoding;
			this->sb->MoveWindow(rect.left, rect.bottom - 30, rect.Width(), 30);
			this->sb->SetPaneInfo(0, IDS_CURRENT, SBPS_NORMAL, currentPosition);
			this->sb->SetPaneInfo(1, IDS_MAGNIFICATION, SBPS_NORMAL, magnification);
			this->sb->SetPaneInfo(2, IDS_CRLF, SBPS_NORMAL, cRLF);
			this->sb->SetPaneInfo(3, IDS_ENCODING, SBPS_NORMAL, encoding);
		}
		else {
			this->textEditor->MoveWindow(MARGIN, MARGIN, rect.Width() - MARGIN, rect.Height() - MARGIN, TRUE);
		}
	}
}

void Notepad::OnGetMinMaxInfo(MINMAXINFO* lpMMI) {
	//클라이언트 최소사이즈 설정
	lpMMI->ptMinTrackSize.x = 420;
	lpMMI->ptMinTrackSize.y = 250;
}

void Notepad::OnMenuClicked(UINT nID) {
	Command* command;
	NotepadCommandFactory commandFactory(this);

	command = commandFactory.Create(nID);

	if (command != 0) {
		command->Execute();
		delete command;
	}
}

void Notepad::OnTextEditorMenuClicked(UINT nID) {
	this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(nID, NULL));
}

void Notepad::OnUpdateMenu(CCmdUI* pCmdUI) {
	Glyph* firstRow = this->textEditor->note->GetAt(0);
	bool isSelecting = this->textEditor->document->isSelecting;
	unsigned int priority_list = CF_TEXT;

	switch (pCmdUI->m_nID) {

		case IDM_EDIT_UNDO:

			if (this->textEditor->unDoCommands == 0 ||
				(this->textEditor->unDoCommands != 0 && this->textEditor->unDoCommands->GetLength() == 0)) {
				pCmdUI->Enable(FALSE);
			}
			else {
				pCmdUI->Enable(TRUE);
			}
			break;

		case IDM_EDIT_REDO:

			if (this->textEditor->reDoCommands == 0 ||
				(this->textEditor->reDoCommands != 0 && this->textEditor->reDoCommands->GetLength() == 0)) {
				pCmdUI->Enable(FALSE);
			}
			else {
				pCmdUI->Enable(TRUE);
			}
			break;

		case IDM_EDIT_CUT:

			if (isSelecting == true) {
				pCmdUI->Enable(TRUE);
			}
			else {
				pCmdUI->Enable(FALSE);
			}
			break;

		case IDM_EDIT_COPY:

			if (isSelecting == true) {
				pCmdUI->Enable(TRUE);
			}
			else {
				pCmdUI->Enable(FALSE);
			}
			break;

		case IDM_EDIT_DELETE:

			if (isSelecting == true) {
				pCmdUI->Enable(TRUE);
			}
			else {
				pCmdUI->Enable(FALSE);
			}
			break;

		case IDM_EDIT_PASTE:

			if (::GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT) {
				pCmdUI->Enable(TRUE);
			}
			else {
				pCmdUI->Enable(FALSE);
			}
			break;

		case IDM_EDIT_FIND:

			if (this->textEditor->note->GetLength() >= 2 || firstRow->GetLength() >= 1) {
				pCmdUI->Enable(TRUE);
			}
			else {
				pCmdUI->Enable(FALSE);
			}
			break;

		case IDM_EDIT_FINDNEXT:

			if (this->textEditor->note->GetLength() >= 2 || firstRow->GetLength() >= 1) {
				pCmdUI->Enable(TRUE);
			}
			else {
				pCmdUI->Enable(FALSE);
			}
			break;

		case IDM_EDIT_FINDPREVIOUS:

			if (this->textEditor->note->GetLength() >= 2 || firstRow->GetLength() >= 1) {
				pCmdUI->Enable(TRUE);
			}
			else {
				pCmdUI->Enable(FALSE);
			}
			break;

		case IDM_EDIT_REPLACE:

			if (this->textEditor->note->GetLength() >= 2 || firstRow->GetLength() >= 1) {
				pCmdUI->Enable(TRUE);
			}
			else {
				pCmdUI->Enable(FALSE);
			}
			break;

		default: break;
	}
}

void Notepad::OnClose(){	
	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_CLOSE, NULL));
	if (this->exit) {
		CFrameWnd::OnClose();
	}
}
