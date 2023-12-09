//ReplaceCurrentCommand.cpp
#include "ReplaceCurrentCommand.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Stack.h"
#include "MacroCommand.h"
#include "Document.h"
#include "Scroll.h"
#include "FileMaker.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ReplaceCurrentCommand::ReplaceCurrentCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = false;
}

ReplaceCurrentCommand::ReplaceCurrentCommand(TextEditor* textEditor, bool isNew, bool isMacro, Command* pCommand) {
	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = isMacro;
	this->pCommand = pCommand;
}

ReplaceCurrentCommand::ReplaceCurrentCommand(TextEditor* textEditor, bool isNew, bool isMacro, Command* pCommand,
	Long row, Long column, Position startPosition, Position endPosition, Long longestRow, Long hSMax, CString findWhat, CString replaceWith, Long replaceWithLength) {

	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = isMacro;
	this->pCommand = pCommand;
	this->row = row;
	this->column = column;
	this->startPosition= startPosition;
	this->endPosition= endPosition;
	this->longestRow = longestRow;
	this->hSMax = hSMax;
	this->findWhat = findWhat;
	this->replaceWith = replaceWith;
	this->replaceWithLength = replaceWithLength;
}

ReplaceCurrentCommand::~ReplaceCurrentCommand() {

}

void ReplaceCurrentCommand::Execute() {
	bool ret = false;
	Long i = 0;
	Long length;
	string replaceWith;
	Command* command;
	FileMaker fm(this->textEditor);

	dynamic_cast<MacroCommand*>(this->pCommand)->unDoReDo = false;
	if (this->isNew) {
		if (this->textEditor->document->isSelecting) {
			if (this->textEditor->isScrolling) {
				this->textEditor->GetBack();
			}
			ret = this->textEditor->document->Compare((LPSTR)(LPCSTR)this->textEditor->findWhat, this->textEditor->document->startPosition, this->textEditor->document->endPosition, this->textEditor->matchCase, &fm);
			if (ret) {
				if (this->textEditor->reDoCommands != 0) {
					delete this->textEditor->reDoCommands;
					this->textEditor->reDoCommands = 0;
				}
				if (this->textEditor->unDoCommands == 0) {
					this->textEditor->unDoCommands = new Stack;
				}
				this->row = this->textEditor->document->GetRowNumber();
				this->column = this->textEditor->note->GetColumnNumber();
				this->startPosition = this->textEditor->document->startPosition;
				this->endPosition = this->textEditor->document->endPosition;
				this->longestRow = this->textEditor->longestRow;
				this->hSMax = this->textEditor->scrollController->horizontalScroll->GetMaximum();
				this->findWhat = this->textEditor->findWhat;
				this->replaceWith = this->textEditor->replaceWith;
				this->replaceWithLength = 0;
				replaceWith = (LPCTSTR)this->replaceWith;
				length = replaceWith.length();
				while (i < length) {
					if (replaceWith[i] >= 0 && replaceWith[i] < ASCII) {
						this->replaceWithLength++;
					}
					else {
						i++;
						this->replaceWithLength++;
					}
					i++;
				}
				dynamic_cast<MacroCommand*>(this->pCommand)->unDoReDo = true;
				command = new ReplaceCurrentCommand(this->textEditor, false, this->isMacro, this->pCommand, this->row, this->column,
					this->startPosition, this->endPosition, this->longestRow, this->hSMax, this->findWhat, this->replaceWith, this->replaceWithLength);
				this->textEditor->unDoCommands->Push(command);
			}
			else {
				dynamic_cast<MacroCommand*>(this->pCommand)->unDoReDo = false;
			}
		}
	}
	else {
		if (this->isMacro) {
			command = new ReplaceCurrentCommand(this->textEditor, false, true, this->pCommand,
				this->row, this->column, this->startPosition, this->endPosition, this->longestRow, this->hSMax, this->findWhat, this->replaceWith, this->replaceWithLength);
			this->textEditor->unDoCommands->Push(command);
		}
		else {
			this->textEditor->unDoCommands->Push(this);
		}
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
		this->textEditor->document->Move(this->startPosition.row, this->startPosition.column);
		dynamic_cast<MacroCommand*>(this->pCommand)->unDoReDo = true;
	}
	if (this->isNew == false || ret) {
		this->textEditor->note->Delete(this->textEditor->document->GetStart(),
			this->startPosition, this->endPosition, this->textEditor->isWrapped, this->textEditor->characterMetrics, this->textEditor->rect.right);
		if (this->textEditor->isWrapped) {
			this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		}
		this->textEditor->note->Paste((LPSTR)(LPCTSTR)this->replaceWith, this->textEditor->characterMetrics);
		if (this->textEditor->isWrapped) {
			this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, true);
		}
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
		else if (this->row != this->textEditor->longestRow) {
			this->textEditor->scrollFlags = SF_COMPARE;
		}
		else if (this->row == this->textEditor->longestRow) {
			if (this->textEditor->current->GetRowLength() < this->hSMax) {
				this->textEditor->scrollFlags = SF_ALL;
			}
			else {
				this->textEditor->scrollFlags = SF_COMPARE;
			}
		}
		this->textEditor->scrollController->UpdateMaximum();
		this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
		this->textEditor->Notify();
		this->textEditor->Invalidate(FALSE);
	}
}

void ReplaceCurrentCommand::Unexecute() {
	Command* command;

	if (this->textEditor->reDoCommands == 0) {
		this->textEditor->reDoCommands = new Stack;
	}
	if (this->isMacro) {
		command = new ReplaceCurrentCommand(this->textEditor, false, true, this->pCommand,
			this->row, this->column, this->startPosition, this->endPosition, this->longestRow, this->hSMax, this->findWhat, this->replaceWith, this->replaceWithLength);
		this->textEditor->reDoCommands->Push(command);
	}
	else {
		this->textEditor->reDoCommands->Push(this);
	}
	if (this->textEditor->isScrolling) {
		this->textEditor->GetBack();
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
	this->textEditor->note->Delete(this->textEditor->document->GetStart(), this->startPosition, Position(this->endPosition.row, this->startPosition.column + this->replaceWithLength),
		this->textEditor->isWrapped, this->textEditor->characterMetrics, this->textEditor->rect.right);
	if (this->textEditor->isWrapped) {
		this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
	}
	this->textEditor->note->Paste((LPSTR)(LPCTSTR)this->findWhat, this->textEditor->characterMetrics);
	if (this->textEditor->isWrapped) {
		this->textEditor->note->WrapRow(this->textEditor->note->GetCurrent() - 1, this->textEditor->rect.right, this->textEditor->characterMetrics, true);
	}
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->document->Select(startPosition, endPosition);
	this->textEditor->document->UpdateSelectingInfo(startPosition, endPosition);
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
		this->textEditor->longestRow = this->longestRow;
		this->textEditor->scrollController->horizontalScroll->SetMaximum(this->hSMax);
		this->textEditor->scrollFlags = SF_NORMAL;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}
