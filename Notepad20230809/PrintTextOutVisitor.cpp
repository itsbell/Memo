//PrintTextOutVisitor.cpp
#include "PrintTextOutVisitor.h"
#include "Glyph.h"
#include "Paper.h"
#include "Row.h"
#include "MultiByteCharacter.h"
#include "SingleByteCharacter.h"
#include "Preview.h"
#include "CharacterMetrics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PrintTextOutVisitor::PrintTextOutVisitor(Preview* preview, HDC hDC, LONG startX, LONG startY) {
	this->preview = preview;
	this->hDC = hDC;
	this->startX = startX;
	this->startY = startY;
	this->x = startX;
	this->y = startY;
}

PrintTextOutVisitor::~PrintTextOutVisitor() {

}

void PrintTextOutVisitor::Visit(Paper* paper) {
	Long i = 0;
	Glyph* row;

	while (i < paper->GetLength()) {
		row = paper->GetAt(i);
		row->Accept(this);
		this->x = this->startX;
		this->y += this->preview->characterMetrics->GetHeight();
		i++;
	}
}

void PrintTextOutVisitor::Visit(Row* row) {
	Long i = 0;
	Glyph* character;

	while (i < row->GetLength()) {
		character = row->GetAt(i);
		character->Accept(this);
		i++;
	}
}

void PrintTextOutVisitor::Visit(SingleByteCharacter* singleByteCharacter) {
	if (singleByteCharacter->GetCharacter() != '\t') {
		::TextOut(this->hDC, this->x, this->y, singleByteCharacter->GetContents().c_str(), 1);
	}

	else {
		int l = this->x;
		int t = this->y;
		int r = this->x + this->preview->characterMetrics->GetWidth(TAB);
		int b = this->y + this->preview->characterMetrics->GetHeight();
		DrawText(this->hDC, "\t", 1, CRect(l, t, r, b), DT_EXPANDTABS);
	}

	this->x += this->preview->characterMetrics->GetWidth(singleByteCharacter->GetContents()[0]);
}

void PrintTextOutVisitor::Visit(MultiByteCharacter* multiByteCharacter) {
	TextOut(this->hDC,this->x, this->y, multiByteCharacter->GetContents().c_str(),2);
	this->x += this->preview->characterMetrics->GetWidth(128);
}