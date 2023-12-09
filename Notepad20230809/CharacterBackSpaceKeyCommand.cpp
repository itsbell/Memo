//CharacterBackSpaceKeyCommand.cpp
#include "CharacterBackSpaceKeyCommand.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Stack.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "SingleByteCharacter.h"
#include "Document.h"
#include "Scroll.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CharacterBackSpaceKeyCommand::CharacterBackSpaceKeyCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = false;
}

CharacterBackSpaceKeyCommand::CharacterBackSpaceKeyCommand(TextEditor* textEditor, bool isNew, bool isMacro, Long row, Long column, char* character, Long longestRow, Long hSMax){
	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = isMacro;
	this->row = row;
	this->column = column;
	this->character[0] = character[0];
	this->character[1] = character[1];
	this->longestRow = longestRow;
	this->hSMax = hSMax;
}

CharacterBackSpaceKeyCommand::~CharacterBackSpaceKeyCommand() {

}

void CharacterBackSpaceKeyCommand::Execute() {
	Glyph* character;
	Command* command;

	if (this->textEditor->unDoCommands == 0) {
		this->textEditor->unDoCommands = new Stack;
	}
	if (this->isNew == true) {
		if (this->textEditor->reDoCommands != 0) {
			delete this->textEditor->reDoCommands;
			this->textEditor->reDoCommands = 0;
		}
		this->row = this->textEditor->document->GetRowNumber();
		this->column = this->textEditor->note->GetColumnNumber();
		character = this->textEditor->current->GetAt(this->textEditor->current->GetCurrent() - 1);
		if (dynamic_cast<SingleByteCharacter*>(character)) {
			this->character[0] = character->GetCharacter();
			this->character[1] = '\0';
		}
		else {
			this->character[0] = character->GetCharacters()[0];
			this->character[1] = character->GetCharacters()[1];
		}
		this->longestRow = this->textEditor->longestRow;
		this->hSMax = this->textEditor->scrollController->horizontalScroll->GetMaximum();
		command = new CharacterBackSpaceKeyCommand(this->textEditor, false, false, this->row, this->column, this->character, this->longestRow, this->hSMax);
		this->textEditor->unDoCommands->Push(command);
	}
	else {
		this->textEditor->unDoCommands->Push(this);
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
	}
	this->textEditor->current->Remove(this->column - 1, this->textEditor->characterMetrics);
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
	else if (this->row != this->textEditor->longestRow) {
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

void CharacterBackSpaceKeyCommand::Unexecute() {
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
	this->textEditor->document->Move(this->row, this->column - 1);
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	character = glyphFactory.Create(this->character);
	if (this->textEditor->isWrapped) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	this->textEditor->current->Add(this->column - 1, character, this->textEditor->characterMetrics);
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
		this->textEditor->longestRow = this->longestRow;
		this->textEditor->scrollController->horizontalScroll->SetMaximum(this->hSMax);
		this->textEditor->scrollFlags = SF_NORMAL;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}