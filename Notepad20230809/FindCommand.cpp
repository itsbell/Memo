//FindCommand.cpp
#include "FindCommand.h"
#include "TextEditor.h"

#include <afxdlgs.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FindCommand::FindCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

FindCommand::~FindCommand() {

}

void FindCommand::Execute() {

	//1. 찾기다이얼로그가 없으면
	if (this->textEditor->findDialog == 0) {

		//1.1. 바꾸기다이얼로그가 있으면 없애다.
		if (this->textEditor->findReplaceDialog != 0) {
			delete this->textEditor->findReplaceDialog;
			this->textEditor->findReplaceDialog = 0;
		}
		int flags = FR_DOWN;
		if (this->textEditor->matchCase) {
			flags |= FR_MATCHCASE;
		}
		if (this->textEditor->matchWholeWord) {
			flags |= FR_WHOLEWORD;
		}
		//1.2. 찾기다이얼로그를 만들다.
		this->textEditor->findDialog = new CFindReplaceDialog;
		this->textEditor->findDialog->Create(TRUE, this->textEditor->findWhat, NULL, flags, this->textEditor);
	}

	//2. 찾기다이얼로그가 있으면 포커스를 주다.
	else {
		this->textEditor->findDialog->SetFocus();
	}
	
}

void FindCommand::Unexecute() {

}