//DeleteCommand.cpp
#include "DeleteCommand.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Stack.h"
#include "Scroll.h"
#include "DummyRow.h"
#include "Document.h"
#include "FileMaker.h"
#include "resource.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DeleteCommand::DeleteCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = false;
	this->startPointer = 0;
	this->endPointer = 0;
}

DeleteCommand::DeleteCommand(TextEditor* textEditor, bool macro) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = macro;
	this->startPointer = 0;
	this->endPointer = 0;
}

DeleteCommand::DeleteCommand(TextEditor* textEditor, bool isNew, bool isMacro, Position startPosition, Position endPosition, Long startPointer, Long endPointer, Long longestRow, Long hSMax){
	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = isMacro;
	this->startPosition = startPosition;
	this->endPosition = endPosition;
	this->startPointer = startPointer;
	this->endPointer = endPointer;
	this->longestRow = longestRow;
	this->hSMax = hSMax;
}

DeleteCommand::~DeleteCommand() {
	
}

void DeleteCommand::Execute() {
	Long count;
	Command* command;
	FileMaker fm(this->textEditor);

	if (this->textEditor->unDoCommands == 0) {
		this->textEditor->unDoCommands = new Stack;
	}
	if (this->isNew == true) {
		if (this->textEditor->reDoCommands != 0) {
			delete this->textEditor->reDoCommands;
			this->textEditor->reDoCommands = 0;
		}
		this->startPosition = this->textEditor->document->startPosition;
		this->endPosition = this->textEditor->document->endPosition;
		this->textEditor->document->AddDeletedContents(this->startPosition, this->endPosition, &(this->startPointer), &(this->endPointer), &fm);
		this->longestRow = this->textEditor->longestRow;
		this->hSMax = this->textEditor->scrollController->horizontalScroll->GetMaximum();
		command = new DeleteCommand(this->textEditor, false, this->isMacro, this->startPosition, this->endPosition, this->startPointer, this->endPointer, this->longestRow, this->hSMax);
		this->textEditor->unDoCommands->Push(command);
	}
	else {
		if (this->isMacro) {
			command = new DeleteCommand(this->textEditor, false, true, this->startPosition, this->endPosition, this->startPointer, this->endPointer, this->longestRow, this->hSMax);
			this->textEditor->unDoCommands->Push(command);
		}
		else {
			this->textEditor->unDoCommands->Push(this);
		}
		if (this->textEditor->document->isSelecting) {
			this->textEditor->note->UnSelect();
			this->textEditor->document->isSelecting = false;
		}
	}
	if (this->startPosition.row < this->textEditor->document->GetStart() || this->startPosition.row > this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		if (this->textEditor->document->GetStart() > this->startPosition.row) {
			this->textEditor->memoryController->MoveUp(this->startPosition.row);
		}
		else {
			this->textEditor->memoryController->MoveDown(this->startPosition.row - this->textEditor->note->GetRowCount() + 1);
		}
	}
	if (this->startPosition.row >= this->textEditor->document->GetStart() && this->endPosition.row <= this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1) {
		this->textEditor->note->Delete(this->textEditor->document->GetStart(), this->startPosition, this->endPosition, this->textEditor->isWrapped, this->textEditor->characterMetrics, this->textEditor->rect.right);
		this->textEditor->isUpdated = false;
	}
	else {
		this->textEditor->document->startPosition = this->startPosition;
		this->textEditor->document->endPosition = this->endPosition;
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->document->Delete(this->textEditor->isWrapped, this->textEditor->rect.right, this->textEditor->characterMetrics, &fm);
		if (this->textEditor->isWrapped) {
			this->textEditor->note->UnWrapRow(this->textEditor->note->GetCurrent(), this->textEditor->characterMetrics);
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, false);
		}
		this->textEditor->document->Move(this->startPosition.row, this->startPosition.column);
		this->textEditor->isUpdated = true;
		this->textEditor->scrollController->UpdateFileVSInfo(false);
	}
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->isMoving = false;
	this->textEditor->isScrolling = false;
	this->textEditor->document->isSelecting = false;
	if (this->textEditor->isModified == false) {
		this->textEditor->isModified = true;
		this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - 메모장").c_str());
	}
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollFlags = SF_NORMAL;
	if (this->textEditor->isWrapped) {
		this->textEditor->scrollFlags = SF_WRAP;
	}
	else if(this->startPosition.row <= this->textEditor->longestRow && this->endPosition.row >= this->textEditor->longestRow) {
		this->textEditor->scrollFlags = SF_ALL;
	}
	else if(this->endPosition.row < this->textEditor->longestRow){
		count = this->endPosition.row - this->startPosition.row;
		this->textEditor->longestRow -= count;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}

void DeleteCommand::Unexecute() {
	Long i;
	Long count;
	Glyph* row;
	Command* command;

	if (this->textEditor->reDoCommands == 0) {
		this->textEditor->reDoCommands = new Stack;
	}
	if (this->isMacro == false) {
		this->textEditor->reDoCommands->Push(this);
	}
	else {
		command = new DeleteCommand(this->textEditor, false, true, this->startPosition, this->endPosition, this->startPointer, this->endPointer, this->longestRow, this->hSMax);
		this->textEditor->reDoCommands->Push(command);
	}
	if (this->textEditor->document->isSelecting) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	if (this->startPosition.row < this->textEditor->document->GetStart() || this->startPosition.row > this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		if (this->startPosition.row < this->textEditor->document->GetStart()) {
			this->textEditor->memoryController->MoveUp(this->startPosition.row);
		}
		else {
			this->textEditor->memoryController->MoveDown(this->startPosition.row - this->textEditor->note->GetRowCount() + 1);
		}
	}
	this->textEditor->document->Move(this->startPosition.row, this->startPosition.column);

	//log
	FileMaker fileMaker(this->textEditor);
	CString log;
	log.Format("ㄴ DeleteCommand / Unexecute / PasteFromFile / RowNumber: %d, startPointer: %d, endPointer %d\n", this->textEditor->document->GetRowNumber(), this->startPointer, this->endPointer);
	fileMaker.RecordLog((LPCTSTR)log);
	//

	this->textEditor->document->PasteFromFile(fileMaker.GetDel(), this->startPointer, this->endPointer, this->textEditor->characterMetrics, this->textEditor->isWrapped, this->textEditor->rect.right);
	this->textEditor->document->Select(this->startPosition, this->endPosition);
	this->textEditor->document->SetSelectingInfo(this->startPosition, this->endPosition);
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = false;
	this->textEditor->isScrolling = false;
	if (this->textEditor->isModified == false) {
		this->textEditor->isModified = true;
		this->textEditor->SetWindowText(('*' + this->textEditor->fileName + " - 메모장").c_str());
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