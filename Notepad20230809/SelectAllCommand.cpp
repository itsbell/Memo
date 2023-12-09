//SelectAllCommand.cpp
#include "SelectAllCommand.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "Document.h"
#include "Position.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SelectAllCommand::SelectAllCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

SelectAllCommand::~SelectAllCommand() {

}

void SelectAllCommand::Execute() {
	Long column;

	this->textEditor->isScrolling = false;
	this->textEditor->document->isSelecting = true;
	this->textEditor->isMoving = false;
	if (this->textEditor->isUpdated == false) {
		this->textEditor->memoryController->Save();
		this->textEditor->scrollController->UpdateFileVSInfo(true);
	}
	if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength()) {
		this->textEditor->memoryController->MoveDown(this->textEditor->document->GetLength() - this->textEditor->note->GetRowCount() + 1);
	}
	this->textEditor->document->SelectAll();
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}

void SelectAllCommand::Unexecute() {

}