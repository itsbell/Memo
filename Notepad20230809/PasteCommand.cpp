//PasteCommand.cpp
#include "PasteCommand.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Stack.h"
#include "Scroll.h"
#include "Document.h"
#include "FileMaker.h"
#include "resource.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PasteCommand::PasteCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = false;
}

PasteCommand::PasteCommand(TextEditor* textEditor, bool isNew, bool isMacro) {
	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = isMacro;
}

PasteCommand::PasteCommand(TextEditor* textEditor, bool isNew, bool isMacro,
	Long row, Long column, Long afterRow, Long afterColumn, Long start, Long end, Long longestRow, Long hSMax) {

	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = isMacro;
	this->row = row;
	this->column = column;
	this->afterRow = afterRow;
	this->afterColumn = afterColumn;
	this->startPointer = start;
	this->endPointer = end;
	this->longestRow = longestRow;
	this->hSMax = hSMax;
}

PasteCommand::~PasteCommand() {

}

void PasteCommand::Execute() {
	char* data;
	unsigned int priority = CF_TEXT;
	Long i;
	Long end;
	Long index;
	Long count;
	Glyph* row;
	FILE* file;
	HANDLE hClipboard;
	Command* command;
	FileMaker fm(this->textEditor);
	
	this->hSMax = this->textEditor->scrollController->horizontalScroll->GetMaximum();
	if (this->isNew == true) {
		//1. Ŭ�����忡�� ���ڿ��� ����� �� �ִ°� �ƴϱ� ������ ���� ���ڿ��� ����Ǿ� �ִ��� Ȯ���ϴ�.
		if (::GetPriorityClipboardFormat(&priority, 1) == CF_TEXT) {
			//1.1. Ŭ�����带 ����.
			if (::OpenClipboard(this->textEditor->m_hWnd)) {
				//1.1.1. Ŭ�����忡�� ���ڿ��� ����� �޸��� �ڵ鰪�� ���.
				hClipboard = ::GetClipboardData(CF_TEXT);
				//1.1.2. �ڵ鰪�� ������
				if (hClipboard != NULL) {
					//1.1.2.1. �޸� �ڵ鰪���κ��� �ּҸ� ��´�.
					data = (char*)::GlobalLock(hClipboard);
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
					file = fopen(fm.GetPaste(), "at");
					if (file != NULL) {
						fseek(file, 0, SEEK_END);
						this->startPointer = ftell(file);
						fprintf(file, "%s\r\n", data);
						this->endPointer = ftell(file) ;
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
						i = this->row;
						while (i < this->afterRow) {
							index = this->textEditor->document->GetRowNumber(true, i);
							this->textEditor->note->WrapRow(index - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, false);
							i++;
						}
						index = this->textEditor->document->GetRowNumber(true, i);
						this->textEditor->note->WrapRow(index - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, true);
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
					}
					command = new PasteCommand(this->textEditor, false, isMacro, this->row, this->column, this->afterRow, this->afterColumn, this->startPointer, this->endPointer, this->textEditor->longestRow, this->hSMax);
					this->textEditor->unDoCommands->Push(command);
				}
			}
			//1.2. Ŭ�����带 �ݴ�.
			::CloseClipboard();
		}
	}
	else {
		if (this->isMacro == true) {
			command = new PasteCommand(this->textEditor, false, isMacro, this->row, this->column, this->afterRow, this->afterColumn, this->startPointer, this->endPointer, this->textEditor->longestRow, this->hSMax);
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
		log.Format("�� PasteCommand / Execute / PasteFromFile / RowNumber: %d, startPointer: %d, endPointer %d\n", this->textEditor->document->GetRowNumber(), this->startPointer, this->endPointer);
		fileMaker.RecordLog((LPCTSTR)log);
		//
		this->textEditor->document->PasteFromFile(fm.GetPaste(), this->startPointer, this->endPointer, this->textEditor->characterMetrics, this->textEditor->isWrapped, this->textEditor->rect.right);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = false;
	if (this->textEditor->isModified == false) {
		this->textEditor->isModified = true;
		this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - �޸���").c_str());
	}
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	if (this->textEditor->isWrapped) {
		this->textEditor->scrollFlags = SF_WRAP;
	}
	else {
		i = this->row;
		if (i < this->textEditor->longestRow) {
			count = this->afterRow - this->row;
			this->textEditor->longestRow += count;
		}
		Long max = 0;
		Long longestRow = this->textEditor->longestRow;
		while (i <= this->afterRow) {
			row = this->textEditor->note->GetAt(i - this->textEditor->document->GetStart());
			if (max < row->GetRowLength()) {
				max = row->GetRowLength();
				longestRow = i;
			}
			i++;
		}
		if (this->hSMax < max) {
			this->textEditor->longestRow = longestRow;
			this->textEditor->scrollController->horizontalScroll->SetMaximum(max);
		}
		this->textEditor->scrollFlags = SF_NORMAL;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics); //Notify�� ���� Onsize�� �Ͼ ��� ������ ������ �ʿ�(��ũ���� �޸��鼭 ��)
	this->textEditor->Invalidate(FALSE);
}

void PasteCommand::Unexecute() {
	Long end = this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1;
	Command* command;
	FileMaker fm(this->textEditor);

	//1. textEditor�� reDoCommands�� ������ ���� �����ϴ�.
	if (this->textEditor->reDoCommands == 0) {
		this->textEditor->reDoCommands = new Stack;
	}
	//2. ��ũ��Ŀ�ǵ��̸� �ش� Ŀ�ǵ带 ���� �����Ͽ� textEditor�� reDoCommands���� Push�ϴ�.
	if (this->isMacro == true) {
		command = new PasteCommand(this->textEditor, false, isMacro, this->row, this->column,
			this->afterRow, this->afterColumn, this->startPointer, this->endPointer, this->textEditor->longestRow, this->hSMax);
		this->textEditor->reDoCommands->Push(command);
	}
	//3. ��ũ��Ŀ�ǵ尡 �ƴϸ� textEditor�� reDoCommands���� Push�ϴ�.
	else {
		this->textEditor->reDoCommands->Push(this);
	}
	//4. ���� �� �����̸� ���������ϴ�.
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
	if (this->row >= this->textEditor->document->GetStart() && this->afterRow <= this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1) {
		this->textEditor->note->Delete(this->textEditor->document->GetStart(), Position(this->row, this->column), Position(this->afterRow, this->afterColumn),
			this->textEditor->isWrapped, this->textEditor->characterMetrics, this->textEditor->rect.right);
	}
	else {
		this->textEditor->document->startPosition = Position(this->row, this->column);
		this->textEditor->document->endPosition = Position(this->afterRow, this->afterColumn);
		this->textEditor->document->Delete(this->textEditor->isWrapped, this->textEditor->rect.right, this->textEditor->characterMetrics, &fm);
		this->textEditor->scrollController->UpdateFileVSInfo(false);
	}
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = false;
	this->textEditor->isScrolling = false;
	this->textEditor->document->isSelecting = false;
	if (this->textEditor->isModified == false) {
		this->textEditor->isModified = true;
		this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - �޸���").c_str());
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
