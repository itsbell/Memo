//DeleteKeyCommand.cpp
#include "DeleteKeyCommand.h"
#include "TextEditor.h"
#include "Document.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "DummyRow.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DeleteKeyCommand::DeleteKeyCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

DeleteKeyCommand::~DeleteKeyCommand() {

}

void DeleteKeyCommand::Execute() {

	if (this->textEditor->isScrolling) {
		this->textEditor->GetBack();
	}
	if (this->textEditor->isWrapped) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	if (this->textEditor->current->GetCurrent() < this->textEditor->current->GetLength()) {
		this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_CHARACTERDELETEKEY, NULL));
	}
	else if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength() && this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {
		this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_ROWDELETEKEY, NULL));
	}
	else if (this->textEditor->note->GetCurrent() == this->textEditor->note->GetLength() &&
		this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength() &&
		this->textEditor->document->GetEnd() < this->textEditor->document->GetLength()) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveDown(this->textEditor->document->GetStart() + 1);
		while (dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1))) {
			this->textEditor->note->Previous();
		}
		this->textEditor->note->Previous();
		this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_ROWDELETEKEY, NULL));
	}
	else {
		this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
}

void DeleteKeyCommand::Unexecute() {
	
}