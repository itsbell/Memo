//ZoomInCommand.h
#include "ZoomInCommand.h"
#include "TextEditor.h"
#include "ZoomController.h"
#include "CharacterMetrics.h"
#include "Glyph.h"
#include "MemoryController.h"
#include "ScrollController.h"
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

ZoomInCommand::ZoomInCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ZoomInCommand::~ZoomInCommand() {

}

void ZoomInCommand::Execute() {
	Long end;
	Long max;
	FILE* file;
	Glyph* glyph;
	FileMaker fm(this->textEditor);

	this->textEditor->zoomController->ZoomIn();
	if (this->textEditor->characterMetrics != 0) {
		delete this->textEditor->characterMetrics;
	}
	this->textEditor->characterMetrics = new CharacterMetrics(this->textEditor->GetDC(), this->textEditor->font, this->textEditor->zoomController);

	if (this->textEditor->isScrolling) {
		this->textEditor->GetBack();
	}

	if (this->textEditor->isWrapped) {
		this->textEditor->note->UnWrap(this->textEditor->characterMetrics);
		this->textEditor->note->Wrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	}
	if (this->textEditor->isUpdated == false) {
		this->textEditor->memoryController->SaveRowLength();
		end = this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1;
		this->textEditor->document->SetEnd(end);
	}
	else {
		this->textEditor->memoryController->SaveRowLength(true);
	}
	this->textEditor->scrollController->UpdateFileVSInfo(true);
	if (this->textEditor->isWrapped) {
		this->textEditor->scrollFlags = SF_WRAP;
	}
	else {
		if (this->textEditor->longestRow < this->textEditor->document->GetStart() || this->textEditor->longestRow > this->textEditor->document->GetEnd()) {
			file = fopen(fm.GetFile(), "rt");
			if (file != NULL) {
				fseek(file, (this->textEditor->longestRow - 1) * (12 + MAX) + 5, SEEK_SET);
				fscanf(file, "%d", &max);
				fclose(file);
			}
		}
		else {
			glyph = this->textEditor->note->GetAt(this->textEditor->document->GetRowNumber(true, this->textEditor->longestRow) - 1);
			max = glyph->GetRowLength();
		}
		this->textEditor->scrollController->horizontalScroll->SetMaximum(max);
		this->textEditor->scrollFlags = SF_NORMAL;
	}

	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}

void ZoomInCommand::Unexecute() {

}