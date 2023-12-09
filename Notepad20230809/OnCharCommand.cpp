//OnCharCommand.cpp
#include "OnCharCommand.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "Stack.h"
#include "Document.h"
#include "resource.h"
#include "MemoryController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OnCharCommand::OnCharCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = false;
}

OnCharCommand::OnCharCommand(TextEditor* textEditor, bool isNew, bool macro) {
	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = macro;
}

OnCharCommand::OnCharCommand(TextEditor* textEditor, bool macro, char* character, Long row, Long column) {
	this->textEditor = textEditor;
	this->isNew = false;
	this->isMacro = macro;
	this->character[0] = character[0];
	this->character[1] = character[1];
	this->row = row;
	this->column = column;
}

OnCharCommand::~OnCharCommand() {

}

void OnCharCommand::Execute() {
	Long end;
	Command* command;
	Glyph* character;
	GlyphFactory glyphFactory;

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
		this->character[0] = this->textEditor->character[0];
		this->character[1] = this->textEditor->character[1];
		command = new OnCharCommand(this->textEditor, this->isMacro, this->character, this->row, this->column);
		this->textEditor->unDoCommands->Push(command);
		if (this->textEditor->isScrolling) {
			this->textEditor->GetBack();
		}
	}
	else {
		if (this->textEditor->document->isSelecting == true) {
			this->textEditor->note->UnSelect();
			this->textEditor->document->isSelecting = false;
		}
		if (this->isMacro == true) {
			command = new OnCharCommand(this->textEditor, true, this->character, this->row, this->column);
			this->textEditor->unDoCommands->Push(command);
		}
		else {
			this->textEditor->unDoCommands->Push(this);
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
	character = glyphFactory.Create(this->character);
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	if (this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {
		this->textEditor->current->Add(character, this->textEditor->characterMetrics);
	}
	else {
		this->textEditor->current->Add(this->textEditor->current->GetCurrent(), character, this->textEditor->characterMetrics);
	}
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	if (this->textEditor->isModified == false) {
		this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - 메모장").c_str());
		this->textEditor->isModified = true;
	}
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = false;
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	if (this->textEditor->isWrapped == true) {
		this->textEditor->scrollFlags = SF_WRAP;
	}
	else {
		this->textEditor->scrollFlags = SF_COMPARE;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}

void OnCharCommand::Unexecute() {
	Long end = this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1;
	Command* command;

	if (this->textEditor->reDoCommands == 0) {
		this->textEditor->reDoCommands = new Stack;
	}
	if (this->isMacro == false) {
		this->textEditor->reDoCommands->Push(this);
	}
	else {
		command = new OnCharCommand(this->textEditor, true, this->character, this->row, this->column);
		this->textEditor->reDoCommands->Push(command);
	}
	if (this->textEditor->document->isSelecting == true) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
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
	}
	this->textEditor->document->Move(this->row, this->column);
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	if (this->textEditor->isWrapped == true) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	this->textEditor->current->Remove(this->textEditor->current->GetCurrent(), this->textEditor->characterMetrics);
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
	if (this->textEditor->isWrapped == true) {
		this->textEditor->scrollFlags = SF_WRAP;
	}
	else if (this->textEditor->longestRow != this->row) {
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