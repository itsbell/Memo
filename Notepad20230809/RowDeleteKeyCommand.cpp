//RowDeleteKeyCommand.cpp
#include "RowDeleteKeyCommand.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "Stack.h"
#include "Document.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RowDeleteKeyCommand::RowDeleteKeyCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = false;
}

RowDeleteKeyCommand::RowDeleteKeyCommand(TextEditor* textEditor, Long row, Long column, Long longestRow, Long hSMax) {
	this->textEditor = textEditor;
	this->isNew = false;
	this->isMacro = false;
	this->row = row;
	this->column = column;
	this->longestRow = longestRow;
	this->hSMax = hSMax;
}

RowDeleteKeyCommand::~RowDeleteKeyCommand() {

}

void RowDeleteKeyCommand::Execute() {
	Long i = 0;
	Glyph* next;
	Glyph* character;
	Command* command;

	if (this->isNew) {
		if (this->textEditor->reDoCommands != 0) {
			delete this->textEditor->reDoCommands;
			this->textEditor->reDoCommands = 0;
		}
		if (this->textEditor->unDoCommands == 0) {
			this->textEditor->unDoCommands = new Stack;
		}
		this->row = this->textEditor->document->GetRowNumber();
		this->column = this->textEditor->note->GetColumnNumber();
		this->longestRow = this->textEditor->longestRow;
		this->hSMax = this->textEditor->scrollController->horizontalScroll->GetMaximum();
		command = new RowDeleteKeyCommand(this->textEditor, this->row, this->column, this->longestRow, this->hSMax);
		this->textEditor->unDoCommands->Push(command);
	}

	else {
		this->textEditor->unDoCommands->Push(this);
		if (this->textEditor->document->isSelecting) { //note->GetIsSelecting()에서 document->isSelecting로 23.11.09 변경
			this->textEditor->note->UnSelect();
		}
		if (this->row < this->textEditor->document->GetStart() || this->row + 1 > this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			if (this->row < this->textEditor->document->GetStart()) {
				this->textEditor->memoryController->MoveUp(this->row);
			}
			else {
				this->textEditor->memoryController->MoveDown(this->row + 1 - this->textEditor->note->GetRowCount() + 1);
			}
		}
		this->textEditor->document->Move(this->row, this->column);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	if (this->textEditor->isWrapped) {
		this->textEditor->note->UnWrapRow(this->textEditor->note->GetCurrent(), this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->document->GetRowNumber(true, this->row) - 1);
		this->textEditor->note->UnWrapRow(this->textEditor->note->GetCurrent() + 1, this->textEditor->characterMetrics);
	}
	next = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent());
	while (i < next->GetLength()) {
		character = next->GetAt(i);
		this->textEditor->current->Add(character->Clone(), this->textEditor->characterMetrics);
		i++;
	}
	this->textEditor->note->Remove(this->textEditor->note->GetCurrent());
	this->textEditor->current->Move(this->column);
	if (this->textEditor->isWrapped) {
		this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, true);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = false;
	this->textEditor->isScrolling = false;
	if (this->textEditor->isModified == false) {
		this->textEditor->isModified = true;
		this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - 메모장").c_str());
	}
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	if (this->textEditor->isWrapped) {
		this->textEditor->scrollFlags = SF_WRAP;
	}
	else {
		if (this->textEditor->document->GetRowNumber() + 1 <= this->textEditor->longestRow) {
			this->textEditor->longestRow--;
		}
		this->textEditor->scrollFlags = SF_COMPARE;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}

void RowDeleteKeyCommand::Unexecute() {
	Glyph* row;
	Glyph* character;
	GlyphFactory glyphFactory;

	if (this->textEditor->reDoCommands == 0) {
		this->textEditor->reDoCommands = new Stack;
	}
	this->textEditor->reDoCommands->Push(this);
	if (this->textEditor->document->isSelecting) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	if (this->row < this->textEditor->document->GetStart() || this->row > this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		if (this->row < this->textEditor->document->GetStart()) {
			this->textEditor->memoryController->MoveUp(this->row);
		}
		else {
			this->textEditor->memoryController->MoveDown(this->row - this->textEditor->note->GetRowCount() + 1);
		}
	}
	this->textEditor->document->Move(this->row, this->column);
	if (this->textEditor->isWrapped) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
	}
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	row = glyphFactory.Create((char*)"\r\n");
	while (this->textEditor->current->GetCurrent() < this->textEditor->current->GetLength()) {
		character = this->textEditor->current->GetAt(this->textEditor->current->GetCurrent());
		row->Add(character->Clone(), this->textEditor->characterMetrics);
		this->textEditor->current->Remove(this->column, this->textEditor->characterMetrics);
	}
	this->textEditor->note->Add(this->textEditor->note->GetCurrent(), row);
	this->textEditor->note->Previous();
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->current->Last();
	if (this->textEditor->isWrapped) {
		this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, true);
		Long i = this->textEditor->note->GetCurrent();
		this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent(), this->textEditor->rect.right, this->textEditor->characterMetrics, false);
		this->textEditor->note->Move(i);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = false;
	this->textEditor->isScrolling = false;
	if (this->textEditor->isModified == false) {
		this->textEditor->isModified = true;
		this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - 메모장").c_str());
	}
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	if (this->textEditor->isWrapped) {
		this->textEditor->scrollFlags = SF_WRAP;
	}
	else {
		this->textEditor->longestRow = this->longestRow;
		this->textEditor->scrollController->horizontalScroll->SetMaximum(this->hSMax);
		this->textEditor->scrollFlags = SF_NORMAL;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}