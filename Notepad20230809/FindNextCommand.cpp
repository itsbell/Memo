//FindNextCommand.cpp
#include "FindNextCommand.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Stack.h"
#include "MacroCommand.h"
#include "Document.h"
#include "FileMaker.h"
#include "resource.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FindNextCommand::FindNextCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = false;
}

FindNextCommand::FindNextCommand(TextEditor* textEditor, bool isNew, bool isMacro, Command* pCommand) {
	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = isMacro;
	this->pCommand = pCommand;
}

FindNextCommand::~FindNextCommand() {

}

void FindNextCommand::Execute() {
	char findWhat[512];
	bool found = false;
	Long row;
	Long start;
	Long end;
	Long startRow;
	Long startColumn;
	Long count;
	FileMaker fm(this->textEditor);

	if (this->isNew) {
		strcpy(findWhat, this->textEditor->findWhat);
		if (this->textEditor->isScrolling == false) {
			if (this->textEditor->matchCase == FALSE && this->textEditor->matchWholeWord == FALSE) {
				this->textEditor->note->FindNext(findWhat, &found, &row, &start, &end);
			}
			else if (this->textEditor->matchCase == TRUE && this->textEditor->matchWholeWord == FALSE) {
				this->textEditor->note->FindNextMatchCase(findWhat, &found, &row, &start, &end);
			}
			else if (this->textEditor->matchWholeWord == TRUE && this->textEditor->matchCase == FALSE) {
				this->textEditor->note->FindNextMatchWholeWord(findWhat, &found, &row, &start, &end);
			}
			else {
				this->textEditor->note->FindNextMatchWholeWordAndCase(findWhat, &found, &row, &start, &end);
			}
			if (found) {
				if (this->textEditor->document->isSelecting) {
					this->textEditor->note->UnSelect();
					this->textEditor->document->isSelecting = false;
				}
				row += this->textEditor->document->GetStart() - 1;
			}
		}
		if (found == false) {
			if (this->textEditor->isScrolling) {
				startRow = this->textEditor->row;
				startColumn = this->textEditor->column;
			}
			else {
				if (this->textEditor->isUpdated == false) {
					this->textEditor->memoryController->Save();
					this->textEditor->scrollController->UpdateFileVSInfo(true);
					this->textEditor->document->SetEnd(this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1);
				}
				startRow = this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount();
				startColumn = 0;
			}
			if (this->textEditor->matchCase == FALSE && this->textEditor->matchWholeWord == FALSE) {
				this->textEditor->document->FindNext(findWhat, startRow, startColumn, &found, &row, &start, &end, &fm);
			}
			else if (this->textEditor->matchCase == TRUE && this->textEditor->matchWholeWord == FALSE) {
				this->textEditor->document->FindNextMatchCase(findWhat, startRow, startColumn, &found, &row, &start, &end, &fm);
			}
			else if (this->textEditor->matchWholeWord == TRUE && this->textEditor->matchCase == FALSE) {
				this->textEditor->document->FindNextMatchWholeWord(findWhat, startRow, startColumn, &found, &row, &start, &end, &fm);
			}
			else {
				this->textEditor->document->FindNextMatchWholeWordAndCase(findWhat, startRow, startColumn, &found, &row, &start, &end, &fm);
			}
			if (found) {
				count = this->textEditor->document->GetEnd() - this->textEditor->document->GetStart() + 1;
				count = this->textEditor->note->GetRowCount() - count;
				row += count;
				if (this->textEditor->document->isSelecting) {
					this->textEditor->note->UnSelect();
					this->textEditor->document->isSelecting = false;
				}
				if (row < this->textEditor->document->GetStart() || row > this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1) {
					if (this->textEditor->isUpdated == false) {
						this->textEditor->memoryController->Save();
						this->textEditor->scrollController->UpdateFileVSInfo(true);
					}
					if (row < this->textEditor->document->GetStart()) {
						this->textEditor->memoryController->MoveUp(row);
					}
					else {
						this->textEditor->memoryController->MoveDown(row - this->textEditor->note->GetRowCount() + 1);
					}
				}
			}
		}
		if (found) {
			this->textEditor->document->Move(row, end);
			this->textEditor->document->Select(Position(row, start), Position(row, end));
			this->textEditor->document->UpdateSelectingInfo(Position(row, start), Position(row, end));
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->isMoving = false;
			this->textEditor->isScrolling = false;
			this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
			this->textEditor->y = this->textEditor->GetY();
			this->textEditor->scrollFlags = SF_NORMAL;
			this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
			this->textEditor->Notify();
			this->textEditor->Invalidate(FALSE);
		}
		else {
			CString contents;
			contents.Format("\"%s\"을(를) 찾을 수 없습니다.", this->textEditor->findWhat);
			this->textEditor->MessageBox(contents, "메모장");
		}
	}
	if (this->isMacro == true && dynamic_cast<MacroCommand*>(this->pCommand)->unDoReDo == true) {
		this->textEditor->unDoCommands->Push(new FindNextCommand(this->textEditor, false, true, this->pCommand));
	}
}

void FindNextCommand::Unexecute() {

	if (this->isMacro == true) {
		if (this->textEditor->reDoCommands == 0) {
			this->textEditor->reDoCommands = new Stack;
		}
		this->textEditor->reDoCommands->Push(new FindNextCommand(this->textEditor, false, true, this->pCommand));
	}
}