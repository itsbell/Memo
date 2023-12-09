//FindPreviousCommand.cpp
#include "FindPreviousCommand.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Document.h"
#include "FileMaker.h"
#include "resource.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FindPreviousCommand::FindPreviousCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

FindPreviousCommand::~FindPreviousCommand() {

}

void FindPreviousCommand::Execute() {
	char findWhat[512];
	bool found = false;
	Long row;
	Long start;
	Long end;
	Long startRow;
	Long startColumn = 0;
	Long count;
	FileMaker fm(this->textEditor);

	strcpy(findWhat, this->textEditor->findWhat);
	if (this->textEditor->isScrolling == false) {
		if (this->textEditor->matchCase == FALSE && this->textEditor->matchWholeWord == FALSE) {
			this->textEditor->note->FindPrevious(findWhat, &found, &row, &start, &end);
		}
		else if (this->textEditor->matchCase == TRUE && this->textEditor->matchWholeWord == FALSE) {
			this->textEditor->note->FindPreviousMatchCase(findWhat, &found, &row, &start, &end);
		}
		else if (this->textEditor->matchWholeWord == TRUE && this->textEditor->matchCase == FALSE) {
			this->textEditor->note->FindPreviousMatchWholeWord(findWhat, &found, &row, &start, &end);
		}
		else {
			this->textEditor->note->FindPreviousMatchWholeWordAndCase(findWhat, &found, &row, &start, &end);
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
			startRow = this->textEditor->document->GetStart() - 1;
			char buffer[MAX];
			FILE* file = fopen(fm.GetFile(), "rt");
			Long i = 0;
			Long byte;
			if (file != NULL) {
				fseek(file, (startRow - 1) * (12 + MAX), SEEK_SET);
				fscanf(file, "%d", &byte);
				fseek(file, 8, SEEK_CUR);
				fread(buffer, byte, 1, file);
				while (i < byte) {
					if (buffer[i] < 0 || buffer[i] > 127) {
						i++;
					}
					startColumn++;
					i++;
				}
				fclose(file);
			}
		}
		if (this->textEditor->matchCase == FALSE && this->textEditor->matchWholeWord == FALSE) {
			this->textEditor->document->FindPrevious(findWhat, startRow, startColumn, &found, &row, &start, &end, &fm);
		}
		else if (this->textEditor->matchCase == TRUE && this->textEditor->matchWholeWord == FALSE) {
			this->textEditor->document->FindPreviousMatchCase(findWhat, startRow, startColumn, &found, &row, &start, &end, &fm);
		}
		else if (this->textEditor->matchWholeWord == TRUE && this->textEditor->matchCase == FALSE) {
			this->textEditor->document->FindPreviousMatchWholeWord(findWhat, startRow, startColumn, &found, &row, &start, &end, &fm);
		}
		else {
			this->textEditor->document->FindPreviousMatchWholeWordAndCase(findWhat, startRow, startColumn, &found, &row, &start, &end, &fm);
		}
		if (found) {
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
		this->textEditor->document->Move(row, start);
		this->textEditor->document->Select(Position(row, start), Position(row, end));
		this->textEditor->document->UpdateSelectingInfo(Position(row, end), Position(row, start));
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->isMoving = false;
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

void FindPreviousCommand::Unexecute() {

}