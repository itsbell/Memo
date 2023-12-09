//ReplaceCommand.cpp
#include "ReplaceCommand.h"
#include "TextEditor.h"
#include <afxdlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ReplaceCommand::ReplaceCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ReplaceCommand::~ReplaceCommand() {

}

void ReplaceCommand::Execute() {

	//1. �ٲٱ� ���̾�αװ� ������
	if (this->textEditor->findReplaceDialog == 0) {

		//1.1. ã�� ���̾�αװ� ������ ���ִ�.
		if (this->textEditor->findDialog != 0) {
			delete this->textEditor->findDialog;
			this->textEditor->findDialog = 0;
		}

		int flags = FR_DOWN;
		if (this->textEditor->matchCase) {
			flags |= FR_MATCHCASE;
		}
		if (this->textEditor->matchWholeWord) {
			flags |= FR_WHOLEWORD;
		}

		//1.2. �ٲٱ� ���̾�α׸� �����.
		this->textEditor->findReplaceDialog = new CFindReplaceDialog;
		this->textEditor->findReplaceDialog->Create(
			FALSE, this->textEditor->findWhat, this->textEditor->replaceWith, flags, this->textEditor);
	}

	//2. �ٲٱ� ���̾�αװ� ������ ��Ŀ���� �ִ�.
	else {
		//this->textEditor->findReplaceDialog->Create(FALSE, this->textEditor->findWhat, this->textEditor->replaceWith, FR_DOWN, NULL);
		this->textEditor->findReplaceDialog->SetFocus();
	}

}

void ReplaceCommand::Unexecute() {

}