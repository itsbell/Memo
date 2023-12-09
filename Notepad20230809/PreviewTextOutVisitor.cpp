//PreviewTextOutVisitor.cpp
#include "PreviewTextOutVisitor.h"
#include "Preview.h"
#include "CharacterMetrics.h"
#include "Paper.h"
#include "Row.h"
#include "SingleByteCharacter.h"
#include "MultiByteCharacter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PreviewTextOutVisitor::PreviewTextOutVisitor(Preview* preview, CDC* dc, LONG startX, LONG startY) {
	this->textEditor = preview->textEditor;
	this->preview = preview;
	this->dc = dc;
	this->startX = startX;
	this->startY = startY;
	this->x = startX;
	this->y = startY;
}

PreviewTextOutVisitor::~PreviewTextOutVisitor() {

}

void PreviewTextOutVisitor::Visit(Paper* paper) {
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

void PreviewTextOutVisitor::Visit(Row* row) {
	Long i = 0;
	Glyph* character;

	while (i < row->GetLength()) {
		character = row->GetAt(i);
		character->Accept(this);
		i++;
	}
}

void PreviewTextOutVisitor::Visit(SingleByteCharacter* singleByteCharacter) {
	
	if (singleByteCharacter->GetCharacter() != '\t') {
		this->dc->TextOut(this->x, this->y, singleByteCharacter->GetCharacter());
	}

	else{
		int l = this->x;
		int t = this->y;
		int r = this->x + this->preview->characterMetrics->GetWidth(TAB);
		int b = this->y + this->preview->characterMetrics->GetHeight();
		this->dc->DrawText("\t", CRect(l, t, r, b), DT_EXPANDTABS);
	}

	this->x += this->preview->characterMetrics->GetWidth(singleByteCharacter->GetContents()[0]);
}

void PreviewTextOutVisitor::Visit(MultiByteCharacter* multiByteCharacter) {
	this->dc->TextOut(this->x, this->y, multiByteCharacter->GetContents().c_str());
	this->x += this->preview->characterMetrics->GetWidth(128);
}