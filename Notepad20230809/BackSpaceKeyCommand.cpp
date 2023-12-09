//BackSpaceKeyCommand.cpp
#include "BackSpaceKeyCommand.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "MemoryController.h"
#include "Document.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BackSpaceKeyCommand::BackSpaceKeyCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

BackSpaceKeyCommand::~BackSpaceKeyCommand() {

}

void BackSpaceKeyCommand::Execute() {

	if (this->textEditor->isScrolling) {
		this->textEditor->GetBack();
	}
	//1. 자동줄바꿈 상태이면 현재 줄만 임시해제하다.
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}

	//2. 현재 칸이 처음이 아니면
	if (this->textEditor->current->GetCurrent() > 0) {
		this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_CHARACTERBACKSPACEKEY, NULL));
	}

	//3. 첫번째 줄이 아니고 현재 칸이 처음이면
	else if (this->textEditor->note->GetCurrent() > 1 && this->textEditor->current->GetCurrent() == 0) {
		this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_ROWBACKSPACEKEY, NULL));
	}

	else if (this->textEditor->note->GetCurrent() == 1 && this->textEditor->current->GetCurrent() == 0 && this->textEditor->document->GetStart() > 1) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
		this->textEditor->note->Next();
		this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_ROWBACKSPACEKEY, NULL));
	}
}

void BackSpaceKeyCommand::Unexecute() {

}