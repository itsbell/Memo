//DateTimeCommand.cpp
#include "DateTimeCommand.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "Document.h"
#include "Glyph.h"
#include "FileMaker.h"
#include "Date.h"
#include "Time.h"
#include "Stack.h"
#include "resource.h"

#pragma warning (disable:4996)

DateTimeCommand::DateTimeCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
}

DateTimeCommand::DateTimeCommand(TextEditor* textEditor, bool isNew, bool isMacro) {
	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = isMacro;
}

DateTimeCommand::DateTimeCommand(TextEditor* textEditor, bool isNew, bool isMacro, Long row, Long column, Long afterRow, Long afterColumn, Long startPointer, Long endPointer, Long longestRow, Long hSMax) {
	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = isMacro;
	this->row = row;
	this->column = column;
	this->afterRow = afterRow;
	this->afterColumn = afterColumn;
	this->startPointer = startPointer;
	this->endPointer = endPointer;
	this->longestRow = longestRow;
	this->hSMax = hSMax;
}

DateTimeCommand::~DateTimeCommand() {

}

void DateTimeCommand::Execute() {
	char* data;
	Long i;
	Long end;
	Long count;
	Long index;
	FILE* file;
	Command* command;
	FileMaker fileMaker(this->textEditor);
	Date date = Date::Today();
	Time tm = Time::GetCurrent();
	
	this->hSMax = this->textEditor->scrollController->horizontalScroll->GetMaximum();
	if (this->isNew) {
		static char buff[18];
		sprintf(buff, "%s %s", (char*)date, (char*)tm);
		data = buff;
		if (this->textEditor->reDoCommands != 0) {
			delete this->textEditor->reDoCommands;
			this->textEditor->reDoCommands = 0;
		}
		if (this->textEditor->unDoCommands == 0) {
			this->textEditor->unDoCommands = new Stack;
		}
		if (this->textEditor->isScrolling) {
			this->textEditor->GetBack();
		}
		this->row = this->textEditor->document->GetRowNumber(false, this->textEditor->note->GetCurrent());
		this->column = this->textEditor->note->GetColumnNumber();
		file = fopen(fileMaker.GetPaste(), "at");
		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			this->startPointer = ftell(file);
			fprintf(file, "%s\r\n", data);
			this->endPointer = ftell(file);
			fclose(file);
		}
		if (this->textEditor->isWrapped) {
			this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		}
		this->textEditor->note->Paste(data, this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->afterRow = this->textEditor->document->GetRowNumber(false, this->textEditor->note->GetCurrent());
		this->afterColumn = this->textEditor->note->GetColumnNumber();

		if (this->textEditor->isWrapped) {
			this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
		command = new DateTimeCommand(this->textEditor, false, this->isMacro, this->row, this->column, this->afterRow, this->afterColumn, this->startPointer, this->endPointer, this->textEditor->longestRow, this->hSMax);
		this->textEditor->unDoCommands->Push(command);
	}
	else {
		if (this->isMacro == true) {
			command = new DateTimeCommand(this->textEditor, false, isMacro, this->row, this->column, this->afterRow, this->afterColumn, this->startPointer, this->endPointer, this->textEditor->longestRow, this->hSMax);
			this->textEditor->unDoCommands->Push(command);
		}
		else {
			this->textEditor->unDoCommands->Push(this);
		}
		if (this->textEditor->document->isSelecting) {
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
		//log
		FileMaker fileMaker(this->textEditor);
		CString log;
		log.Format("ㄴ DateTime / Execute / PasteFromFile / RowNumber: %d, startPointer: %d, endPointer %d\n", this->textEditor->document->GetRowNumber(), this->startPointer, this->endPointer);
		fileMaker.RecordLog((LPCTSTR)log);
		//
		this->textEditor->document->PasteFromFile(fileMaker.GetPaste(), this->startPointer, this->endPointer, this->textEditor->characterMetrics, this->textEditor->isWrapped, this->textEditor->rect.right);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = false;
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
		this->textEditor->scrollFlags = SF_COMPARE;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}

void DateTimeCommand::Unexecute() {
	Long end = this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1;
	Command* command;

	if (this->textEditor->reDoCommands == 0) {
		this->textEditor->reDoCommands = new Stack;
	}
	if (this->isMacro == true) {
		command = new DateTimeCommand(this->textEditor, false, isMacro, this->row, this->column, this->afterRow, this->afterColumn, this->startPointer, this->endPointer, this->textEditor->longestRow, this->hSMax);
		this->textEditor->reDoCommands->Push(command);
	}
	else {
		this->textEditor->reDoCommands->Push(this);
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
	this->textEditor->note->Delete(this->textEditor->document->GetStart(), Position(this->row, this->column), Position(this->afterRow, this->afterColumn), 
		this->textEditor->isWrapped, this->textEditor->characterMetrics, this->textEditor->rect.right);
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = false;
	this->textEditor->isScrolling = false;
	this->textEditor->document->isSelecting = false;
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
