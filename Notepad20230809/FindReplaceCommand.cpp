//FindReplaceCommand.cpp
#include "FindReplaceCommand.h"
#include "TextEditor.h"
#include "resource.h"
#include <afxdlgs.h>

FindReplaceCommand::FindReplaceCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

FindReplaceCommand::~FindReplaceCommand() {

}

void FindReplaceCommand::Execute() {

	CFindReplaceDialog* pDlg = pDlg->GetNotifier(this->textEditor->lParam);

	if (pDlg != NULL) {

		if (pDlg->FindNext()) {
			this->textEditor->findWhat = pDlg->m_fr.lpstrFindWhat;
			this->textEditor->replaceWith = pDlg->m_fr.lpstrReplaceWith;
			this->textEditor->matchCase = pDlg->MatchCase();
			this->textEditor->matchWholeWord = pDlg->MatchWholeWord();

			if (pDlg->SearchDown()) {
				this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_FINDNEXT, NULL));
			}

			else {
				this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_FINDPREVIOUS, NULL));
			}
		}

		else if (pDlg->ReplaceCurrent()) {
			this->textEditor->findWhat = pDlg->m_fr.lpstrFindWhat;
			this->textEditor->replaceWith = pDlg->m_fr.lpstrReplaceWith;
			this->textEditor->matchCase = pDlg->MatchCase();
			this->textEditor->matchWholeWord = pDlg->MatchWholeWord();

			this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_REPLACEFIND, NULL));
		}

		else if (pDlg->ReplaceAll()) {
			this->textEditor->findWhat = pDlg->m_fr.lpstrFindWhat;
			this->textEditor->replaceWith = pDlg->m_fr.lpstrReplaceWith;
			this->textEditor->matchCase = pDlg->MatchCase();
			this->textEditor->matchWholeWord = pDlg->MatchWholeWord();

			this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_REPLACEALL, NULL));
		}

		pDlg->SetFocus();

		if (pDlg->IsTerminating()) {

			if (this->textEditor->findDialog != 0) {
				this->textEditor->findDialog = 0;
			}

			if (this->textEditor->findReplaceDialog != 0) {
				this->textEditor->findReplaceDialog = 0;
			}
		}
	}
}

void FindReplaceCommand::Unexecute() {

}