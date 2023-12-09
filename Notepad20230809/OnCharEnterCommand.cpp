//OnCharEnterCommand.cpp
#include "OnCharEnterCommand.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "Stack.h"
#include "Document.h"
#include "Scroll.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OnCharEnterCommand::OnCharEnterCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = false;
}

OnCharEnterCommand::OnCharEnterCommand(TextEditor* textEditor, bool macro) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = macro;
}

OnCharEnterCommand::OnCharEnterCommand(TextEditor* textEditor, bool macro, Long row, Long column) {
	this->textEditor = textEditor;
	this->isNew = false;
	this->isMacro = macro;
	this->row = row;
	this->column = column;
}

OnCharEnterCommand::~OnCharEnterCommand(){

}

void OnCharEnterCommand::Execute() {
	Long end;
	Glyph* glyph;
	Glyph* character;
	GlyphFactory glyphFactory;
	Command* command;

	if (this->textEditor->unDoCommands == 0) {
		this->textEditor->unDoCommands = new Stack;
	}
	if (this->isNew == true) {
		if (this->textEditor->reDoCommands != 0) {
			delete this->textEditor->reDoCommands;
			this->textEditor->reDoCommands = 0;
		}
		if (this->textEditor->isScrolling == true) {
			this->textEditor->GetBack();
		}
		this->row = this->textEditor->document->GetRowNumber();
		this->column = this->textEditor->note->GetColumnNumber();
		command = new OnCharEnterCommand(this->textEditor, this->isMacro, this->row, column);
		this->textEditor->unDoCommands->Push(command);
	}
	else {
		if (this->isMacro == true) {
			command = new OnCharEnterCommand(this->textEditor, true, this->row, this->column);
			this->textEditor->unDoCommands->Push(command);
		}
		else {
			this->textEditor->unDoCommands->Push(this);
		}
		if (this->textEditor->document->isSelecting == true) {
			this->textEditor->note->UnSelect();
			this->textEditor->document->isSelecting = false;
		}
		end = this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1;
		if (this->row < this->textEditor->document->GetStart() || this->row > end) {
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
			this->textEditor->isScrolling = false;
		}
		this->textEditor->document->Move(this->row, this->column);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	glyph = glyphFactory.Create((char*)"\r\n");
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	while (this->textEditor->current->GetCurrent() < this->textEditor->current->GetLength()) {
		character = this->textEditor->current->GetAt(this->textEditor->current->GetCurrent());
		glyph->Add(character->Clone(), this->textEditor->characterMetrics);
		this->textEditor->current->Remove(this->textEditor->current->GetCurrent(), this->textEditor->characterMetrics);
	}
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
	}
	glyph->First();
	this->textEditor->note->Add(this->textEditor->note->GetCurrent(), glyph);
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
	}
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = false;
	if (this->textEditor->isModified == false) {
		this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - 메모장").c_str());
		this->textEditor->isModified = true;
	}
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	if (this->textEditor->isWrapped == true) {
		this->textEditor->scrollFlags = SF_WRAP;
	}
	else if (this->row != this->textEditor->longestRow) {
		if (this->row < this->textEditor->longestRow) {
			this->textEditor->longestRow++;
		}
		this->textEditor->scrollFlags = SF_NORMAL;
	}
	else {
		this->textEditor->scrollFlags = SF_ALL;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}

void OnCharEnterCommand::Unexecute() {
	Long i = 0;
	Long column;
	Long end = this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1;
	Glyph* current;
	Glyph* next;
	Glyph* character;
	Command* command;

	if (this->textEditor->reDoCommands == 0) {
		this->textEditor->reDoCommands = new Stack;
	}
	if (this->isMacro == true) {
		command = new OnCharEnterCommand(this->textEditor, true, this->row, this->column);
		this->textEditor->reDoCommands->Push(command);
	}
	else {
		this->textEditor->reDoCommands->Push(this);
	}
	if (this->textEditor->document->isSelecting == true) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	if (this->row < this->textEditor->document->GetStart() || this->row + 1 > end) {
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
	current = this->textEditor->current;
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->note->Next();
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		next = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	else {
		this->textEditor->note->Next();
		next = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	column = current->GetCurrent();
	while (i < next->GetLength()) {
		character = next->GetAt(i);
		current->Add(character->Clone(), this->textEditor->characterMetrics);
		i++;
	}
	this->textEditor->note->Remove(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->current->Move(column);
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	if (this->textEditor->isModified == false) {
		this->textEditor->isModified = true;
		this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - 메모장").c_str());
	}
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = false;
	this->textEditor->isScrolling = false;
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollFlags = SF_COMPARE;
	if (this->textEditor->isWrapped == true) {
		this->textEditor->scrollFlags = SF_WRAP;
	}
	else if (this->row < this->textEditor->longestRow) {
		this->textEditor->longestRow--;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}
