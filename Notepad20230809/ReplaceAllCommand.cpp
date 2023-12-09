//ReplaceAllCommand.cpp
#include "ReplaceAllCommand.h"
#include "TextEditor.h"
#include "Document.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Stack.h"
#include "Position.h"
#include "ReplaceInfoStack.h"
#include "Scroll.h"
#include "GlyphFactory.h"
#include "FileMaker.h"
#include "resource.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ReplaceAllCommand::ReplaceAllCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->unDoStack = 0;
	this->reDoStack = 0;
	this->isNew = true;
	this->isMacro = false;
	this->findWhat = textEditor->findWhat;
	this->replaceWith = textEditor->replaceWith;
	this->hSMax = 0;
}

ReplaceAllCommand::ReplaceAllCommand(const ReplaceAllCommand& source) {
	this->textEditor = source.textEditor;

	if (source.unDoStack != 0) {
		this->unDoStack = new ReplaceInfoStack(*(source.unDoStack));
	}
	
	else {
		this->unDoStack = 0;
	}

	if (source.reDoStack != 0) {
		this->reDoStack = new ReplaceInfoStack(*(source.reDoStack));
	}

	else {
		this->reDoStack = 0;
	}
	
	this->isNew = false;
	this->isMacro = source.isMacro;
	this->findWhat = source.findWhat;
	this->replaceWith = source.replaceWith;
	this->longestRow = source.longestRow;
	this->hSMax = source.hSMax;
}

ReplaceAllCommand::~ReplaceAllCommand() {

	if (this->unDoStack != 0) {
		delete this->unDoStack;
	}

	if (this->reDoStack != 0) {
		delete this->reDoStack;
	}
}

void ReplaceAllCommand::Execute() {
	char buffer[MAX - 2];
	char contents[MAX - 2];
	char findWhatInText[512];
	char* findWhat;
	char* replaceWith;
	bool found;
	Long i = 1;
	Long j = 0;
	Long k;
	Long byte;
	Long index = 0;
	Long start;
	Long end;
	Long rowLength;
	Long count = 0;
	Long findWhatLength = 0;
	FILE* file;
	Position* position;
	FileMaker fm(this->textEditor);
	GlyphFactory glyphFactory;

	if (this->isNew) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		if (this->textEditor->document->isSelecting) {
			this->textEditor->note->UnSelect();
			this->textEditor->document->isSelecting = false;
		}
		this->findWhat = this->textEditor->findWhat;
		this->replaceWith = this->textEditor->replaceWith;
		findWhat = (LPSTR)(LPCTSTR)this->findWhat;
		replaceWith = (LPSTR)(LPCTSTR)this->replaceWith;
		file = fopen(fm.GetFile(), "rt+");
		if (file != NULL) {
			while (i <= this->textEditor->document->GetLength()) {
				fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
				fscanf(file, "%d", &byte);
				fseek(file, 8, SEEK_CUR);
				fread(buffer, byte, 1, file);
				buffer[byte] = '\0';
				if (this->textEditor->matchCase == false && this->textEditor->matchWholeWord == false) {
					this->textEditor->document->FindNext(buffer, index, findWhat, &found, &start, &end);
				}
				else if (this->textEditor->matchCase == false && this->textEditor->matchWholeWord) {
					this->textEditor->document->FindNextMatchWholeWord(buffer, index, findWhat, &found, &start, &end);
				}
				else if (this->textEditor->matchCase && this->textEditor->matchWholeWord == false) {
					this->textEditor->document->FindNextMatchCase(buffer, index, findWhat, &found, &start, &end);
				}
				else {
					this->textEditor->document->FindNextMatchWholeWordAndCase(buffer, index, findWhat, &found, &start, &end);
				}
				if (found) {
					count++;
					if (count == 1) {
						k = start;
						while (j <= end - start) {
							findWhatInText[j] = buffer[k];
							k++;
							j++;
						}
						findWhatInText[j] = '\0';
						this->findWhat.Format("%s", findWhatInText);
						if (this->textEditor->unDoCommands == 0) {
							this->textEditor->unDoCommands = new Stack;
						}
						if (this->textEditor->reDoCommands != 0) {
							delete this->textEditor->reDoCommands;
							this->textEditor->reDoCommands = 0;
						}
						if (this->unDoStack == 0) {
							this->unDoStack = new ReplaceInfoStack;
						}
						this->longestRow = this->textEditor->longestRow;
						this->hSMax = this->textEditor->scrollController->horizontalScroll->GetMaximum();
						if (this->textEditor->isWrapped) {
							this->textEditor->scrollFlags = SF_WRAP;
						}
						else {
							this->textEditor->scrollFlags = SF_NORMAL;
						}
						if (this->textEditor->isModified == false) {
							this->textEditor->isModified = true;
							this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - 메모장").c_str());
						}
					}
					this->unDoStack->Push(new Position(i, start));
					this->textEditor->document->Correct(buffer, start, end, this->textEditor->characterMetrics, contents, replaceWith, &byte, &rowLength, &index);
					fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
					fprintf(file, "%04d %05d\n", byte, rowLength);
					fwrite(contents, MAX - 2, 1, file);
					if (i < this->textEditor->document->GetLength()) {
						fputs("\n", file);
					}
					if (this->textEditor->scrollFlags == SF_NORMAL && i != this->textEditor->longestRow) {
						if (rowLength > this->textEditor->scrollController->horizontalScroll->GetMaximum()) {
							this->textEditor->longestRow = i;
							this->textEditor->scrollController->horizontalScroll->SetMaximum(rowLength);
						}
					}
					else if (this->textEditor->scrollFlags != SF_WRAP && i == this->textEditor->longestRow) {
						this->textEditor->scrollFlags = SF_ALL;
					}
				}
				else {
					index = 0;
					i++;
				}
			}
			fclose(file);
		}
		if (count > 0) {
			this->textEditor->unDoCommands->Push(new ReplaceAllCommand(*this));
		}
	}
	else {
		if (this->textEditor->unDoCommands == 0) {
			this->textEditor->unDoCommands = new Stack;
		}
		count = this->reDoStack->GetLength();
		if (this->textEditor->isWrapped) {
			this->textEditor->scrollFlags = SF_WRAP;
		}
		else {
			this->textEditor->scrollFlags = SF_NORMAL;
		}
		if (this->textEditor->document->isSelecting) {
			this->textEditor->note->UnSelect();
			this->textEditor->document->isSelecting = false;
		}
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		findWhat = (LPSTR)(LPCTSTR)this->findWhat;
		replaceWith = (LPSTR)(LPCTSTR)this->replaceWith;
		i = 0;
		while (findWhat[i] != '\0') {
			findWhatLength++;
			i++;
		}
		file = fopen(fm.GetFile(), "rt+");
		if (file != NULL) {
			while (this->reDoStack->GetLength() > 0) {
				position = this->reDoStack->Pop();
				fseek(file, (position->row - 1) * (MAX + 12), SEEK_SET);
				fscanf(file, "%d", &byte);
				fseek(file, 8, SEEK_CUR);
				fread(buffer, byte, 1, file);
				buffer[byte] = '\0';
				this->textEditor->document->Correct(buffer, position->column, position->column + findWhatLength - 1, this->textEditor->characterMetrics, contents, replaceWith, &byte, &rowLength, &index);
				fseek(file, (position->row - 1) * (12 + MAX), SEEK_SET);
				fprintf(file, "%04d %05d\n", byte, rowLength);
				fwrite(contents, MAX - 2, 1, file);
				if (position->row < this->textEditor->document->GetLength()) {
					fputs("\n", file);
				}
				if (this->textEditor->scrollFlags == SF_NORMAL && position->row != this->textEditor->longestRow) {
					if (rowLength > this->textEditor->scrollController->horizontalScroll->GetMaximum()) {
						this->textEditor->longestRow = position->row;
						this->textEditor->scrollController->horizontalScroll->SetMaximum(rowLength);
					}
				}
				else if (this->textEditor->scrollFlags != SF_WRAP && position->row == this->textEditor->longestRow) {
					this->textEditor->scrollFlags = SF_ALL;
				}
				this->unDoStack->Push(position);
			}
			fclose(file);
		}
		this->textEditor->unDoCommands->Push(this);
		if (this->textEditor->isModified == false) {
			this->textEditor->isModified = true;
			this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - 메모장").c_str());
		}
	}
	if (this->textEditor->note != 0) {
		delete this->textEditor->note;
	}
	this->textEditor->note = glyphFactory.Create((char*)"\0");
	this->textEditor->document->paper = this->textEditor->note;

	if (this->textEditor->isWrapped) {
		this->textEditor->document->LoadToMemory(1, OPTIMAL,this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
	}
	else {
		this->textEditor->document->LoadToMemory(1, OPTIMAL, this->textEditor->characterMetrics, &fm);
	}
	this->textEditor->document->SetStart(1);
	this->textEditor->document->SetEnd(this->textEditor->note->GetRowCount());
	this->textEditor->scrollController->UpdateFileVSInfo(true);
	this->textEditor->note->First();
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->current->First();
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = true;
	this->textEditor->isScrolling = false;
	this->textEditor->x = 0;
	this->textEditor->y = 0;
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
	CString message;
	message.Format("%d개가 바뀌었습니다.", count);
	this->textEditor->MessageBox(message, "모두 바꾸기", MB_OK);
}

void ReplaceAllCommand::Unexecute() {
	char buffer[MAX - 2];
	char contents[MAX - 2];
	char* findWhat;
	char* replaceWith;
	Long i = 0;
	Long byte;
	Long index;
	Long rowLength;
	Long replaceWithLength = 0;
	FILE* file;
	Position* position;
	FileMaker fm(this->textEditor);
	GlyphFactory glyphFactory;

	findWhat = (LPSTR)(LPCTSTR)this->findWhat;
	replaceWith = (LPSTR)(LPCTSTR)this->replaceWith;
	while (replaceWith[i] != '\0') {
		i++;
	}
	replaceWithLength = i;
	if (this->textEditor->reDoCommands == 0) {
		this->textEditor->reDoCommands = new Stack;
	}
	if (this->reDoStack == 0) {
		this->reDoStack = new ReplaceInfoStack;
	}
	if (this->textEditor->document->isSelecting) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	if (this->textEditor->isUpdated == false) {
		this->textEditor->memoryController->Save();
		this->textEditor->scrollController->UpdateFileVSInfo(true);
	}
	file = fopen(fm.GetFile(), "rt+");
	if (file != NULL) {
		while (this->unDoStack->GetLength() > 0) {
			position = this->unDoStack->Pop();
			fseek(file, (position->row - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			buffer[byte] = '\0';
			this->textEditor->document->Correct(buffer, position->column, position->column + replaceWithLength - 1, this->textEditor->characterMetrics, contents, findWhat, &byte, &rowLength, &index);
			fseek(file, (position->row - 1) * (12 + MAX), SEEK_SET);
			fprintf(file, "%04d %05d\n", byte, rowLength);
			fwrite(contents, MAX - 2, 1, file);
			if (position->row < this->textEditor->document->GetLength()) {
				fputs("\n", file);
			}
			this->reDoStack->Push(position);
		}
		fclose(file);
	}
	this->textEditor->reDoCommands->Push(this);
	if (this->textEditor->note != 0) {
		delete this->textEditor->note;
	}
	this->textEditor->note = glyphFactory.Create((char*)"\0");
	this->textEditor->document->paper = this->textEditor->note;
	if (this->textEditor->isWrapped) {
		this->textEditor->document->LoadToMemory(1, OPTIMAL, this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
	}
	else {
		this->textEditor->document->LoadToMemory(1, OPTIMAL, this->textEditor->characterMetrics, &fm);
	}
	this->textEditor->document->SetStart(1);
	this->textEditor->document->SetEnd(this->textEditor->note->GetRowCount());
	this->textEditor->scrollController->UpdateFileVSInfo(true);
	this->textEditor->note->First();
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->current->First();
	this->textEditor->isMoving = false;
	this->textEditor->isUpdated = true;
	this->textEditor->isScrolling = false;
	if (this->textEditor->isModified == false) {
		this->textEditor->isModified = true;
		this->textEditor->GetParent()->SetWindowText(('*' + this->textEditor->fileName + " - 메모장").c_str());
	}
	this->textEditor->x = 0;
	this->textEditor->y = 0;
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