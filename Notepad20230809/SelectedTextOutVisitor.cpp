//SelectedTextOutVisitor.cpp

#include "SelectedTextOutVisitor.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "Glyph.h"
#include "Paper.h"
#include "Row.h"
#include "SingleByteCharacter.h"
#include "MultiByteCharacter.h"
#include "CharacterMetrics.h"
#include "DummyRow.h"
#include "Document.h"
#include "Position.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SelectedTextOutVisitor::SelectedTextOutVisitor(TextEditor* textEditor, CDC* dc, int y, Long start, Long end) {
	this->textEditor = textEditor;
	this->dc = dc;
	this->x = 0 - hSPosition;
	this->y = y;
	this->start = start;
	this->end = end;
	this->hSPosition = textEditor->scrollController->horizontalScroll->GetPosition();
	this->hSPage = textEditor->scrollController->horizontalScroll->GetPage();
	this->characterHeight = textEditor->characterMetrics->GetHeight();
}

SelectedTextOutVisitor::~SelectedTextOutVisitor() {

}

void SelectedTextOutVisitor::Visit(Paper* paper) {
	Long i = this->start;
	Glyph* row;
	Glyph* next;

	while (i < this->end && i < paper->GetLength()) {
		row = paper->GetAt(i);
		row->Accept(this);
		if (i < this->textEditor->note->GetLength() - 1) {
			next = paper->GetAt(i + 1);
			if (next->GetIsSelected()) {
				this->dc->TextOut(row->GetRowLength() - this->hSPosition, this->y, " ");
			}
		}
		else {
			if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength() &&
				this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() > this->textEditor->document->startPosition.row &&
				this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() <= this->textEditor->document->endPosition.row) {
				row = paper->GetAt(paper->GetLength() - 1);
				this->dc->TextOut(row->GetRowLength()-this->hSPosition, this->y, " ");
			}
		}
		this->y += this->textEditor->characterMetrics->GetHeight();
		i++;
	}
}

void SelectedTextOutVisitor::Visit(Row* row) {
	Long i = 0;
	Long sum = 0;
	Glyph* character = 0;

	this->x = 0;
	//1. 문자 수 만큼 반복하다.
	while (i < row->GetLength() && sum <= this->hSPosition) {
		character = row->GetAt(i);
		sum += character->GetWidth(this->textEditor->characterMetrics);
		i++;
	}

	if (sum > this->hSPosition) {
		if (this->hSPosition > 0) {
			this->x = sum - this->hSPosition - character->GetWidth(this->textEditor->characterMetrics);
		}
		i--;
		while (i < row->GetLength() && this->x < this->hSPage) {
			character = row->GetAt(i);
			character->Accept(this);
			i++;
		}
	}
}

void SelectedTextOutVisitor::Visit(SingleByteCharacter* singleByteCharacter) {

	if (singleByteCharacter->GetIsSelected() == true && singleByteCharacter->GetCharacter() != '\t' && singleByteCharacter->GetCharacter() != 'j') {
		this->dc->TextOut(this->x, this->y, singleByteCharacter->GetContents()[0]);
	}
	else if(singleByteCharacter->GetIsSelected() == true && singleByteCharacter->GetCharacter() == 'j') {
		int l = this->x;
		int t = this->y;
		int r = this->x + this->textEditor->characterMetrics->GetWidth('j');
		int b = this->y + this->textEditor->characterMetrics->GetHeight();

		this->dc->DrawText("j", CRect(l, t, r, b), DT_EXPANDTABS);
	}
	else if (singleByteCharacter->GetIsSelected() == true && singleByteCharacter->GetCharacter() == '\t') {
		int l = this->x;
		int t = this->y;
		int r = this->x + this->textEditor->characterMetrics->GetWidth(TAB);
		int b = this->y + this->textEditor->characterMetrics->GetHeight();

		this->dc->DrawText("\t", CRect(l, t, r, b), DT_EXPANDTABS);
	}

	this->x += this->textEditor->characterMetrics->GetWidth(singleByteCharacter->GetContents()[0]);
}

void SelectedTextOutVisitor::Visit(MultiByteCharacter* multiByteCharacter) {
	
	if (multiByteCharacter->GetIsSelected() == true) {
		this->dc->TextOut(this->x, this->y, multiByteCharacter->GetContents().c_str());
	}
	
	this->x += this->textEditor->characterMetrics->GetWidth(128);
}