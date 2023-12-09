//TextOutVisitor.cpp

#include "TextOutVisitor.h"
#include "TextEditor.h"
#include "Paper.h"
#include "Row.h"
#include "SingleByteCharacter.h"
#include "MultiByteCharacter.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "Document.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Ŭ������: TextOutVisitor
��    ��: TextOutVisitor -�� Visitor / �ؽ�Ʈ�� ��ü ����ϴ� Ŭ���� / ���� ����������(Visitor ����������)
*/

/*
�Լ��̸�: TextOutVisitor
��    ��: TextOutVisitor �Ű������� ������ ������ (CDCŬ������ TextOut�Լ��� ���� ���� CDC*�� �Ű������� ������)
��    ��: CDC Link
��    ��: ����
*/
TextOutVisitor::TextOutVisitor(TextEditor* textEditor, CDC* dc, int y, Long start, Long end) {
	this->textEditor = textEditor;
	this->dc = dc;
	this->x = 0;
	this->y = y;
	this->start = start;
	this->end = end;
	this->hSPosition = textEditor->scrollController->horizontalScroll->GetPosition();
	this->hSPage = textEditor->scrollController->horizontalScroll->GetPage();
	this->characterHeight = textEditor->characterMetrics->GetHeight();
}

TextOutVisitor::~TextOutVisitor() {

}

/*
�Լ��̸�: Visit
��    ��: ���� ������ ȭ�鿡 ����ϴ�.
��    ��: ����
��    ��: ����
*/
void TextOutVisitor::Visit(Paper* paper) {
	Long i = this->start;
	Glyph* row;

	while (i < this->end && i < paper->GetLength()) {
		row = paper->GetAt(i);
		//1.1. �� ������ ��� ����ϴ�.
		row->Accept(this);
		//1.3. Y��ǥ�� ������ ���� ��ŭ �̵��ϴ�.
		this->y += this->textEditor->characterMetrics->GetHeight();
		i++;
	}
}

/*
�Լ��̸�: Visit
��    ��: �� ������ ȭ�鿡 ����ϴ�.
��    ��: ��
��    ��: ����
*/
void TextOutVisitor::Visit(Row* row) {
	Long i = 0;
	Long sum = 0;
	Glyph* character = 0;
	
	//1. ���� �� ��ŭ �ݺ��ϴ�.
	while (i < row->GetLength() && sum <= this->hSPosition) {
		character = row->GetAt(i);
		sum += character->GetWidth(this->textEditor->characterMetrics);
		i++;
	}
	
	if (sum > this->hSPosition) {
		if (this->hSPosition == 0) {
			this->x = 0;
		}
		else {
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

/*
�Լ��̸�: Visit
��    ��: ���� ������ ȭ�鿡 ����ϴ�.
��    ��: ����
��    ��: ����
*/
void TextOutVisitor::Visit(SingleByteCharacter* singleByteCharacter) {
 //����
	//1. ���ڸ� ����ϴ�.
	if (singleByteCharacter->GetCharacter() != '\t') {
		this->dc->TextOut(this->x, this->y, singleByteCharacter->GetCharacter());
		
	}
	else {
		int l = this->x;
		int t = this->y;
		int r = this->x + this->textEditor->characterMetrics->GetWidth(TAB);
		int b = this->y + this->textEditor->characterMetrics->GetHeight();

		this->dc->DrawText("\t", CRect(l, t, r, b), DT_EXPANDTABS);
	}

	//2. ������ �ʺ�ŭ �����ġ�� �̵��ϴ�.
	this->x += this->textEditor->characterMetrics->GetWidth(singleByteCharacter->GetCharacter());


#if 0 // DrawText (��ġ�� ������ �Ϻΰ� �߸�)
	//1. ���ڸ� ����ϴ�.
	if (singleByteCharacter->GetCharacter() != '\t' && singleByteCharacter->GetCharacter() != 'j') {
		this->dc->TextOut(this->x, this->y, singleByteCharacter->GetCharacter()); 
	}
	else if (singleByteCharacter->GetCharacter() == 'j') {
		int l = this->x;
		int t = this->y;
		int r = this->x + this->textEditor->characterMetrics->GetWidth('j');
		int b = this->y + this->textEditor->characterMetrics->GetHeight();

		this->dc->DrawText("j", CRect(l, t, r, b), DT_EXPANDTABS);
	}
	else {
		int l = this->x;
		int t = this->y;
		int r = this->x + this->textEditor->characterMetrics->GetWidth(TAB);
		int b = this->y + this->textEditor->characterMetrics->GetHeight();

		this->dc->DrawText("\t", CRect(l, t, r, b), DT_EXPANDTABS);
	}
	
	//2. ������ �ʺ�ŭ �����ġ�� �̵��ϴ�.
	this->x += this->textEditor->characterMetrics->GetWidth(singleByteCharacter->GetCharacter());
#endif
}

/*
�Լ��̸�: Visit
��    ��: ���� ������ ȭ�鿡 ����ϴ�.
��    ��: ����
��    ��: ����
*/
void TextOutVisitor::Visit(MultiByteCharacter* multiByteCharacter) {

	//1. ���ڸ� ����ϴ�.
	this->dc->TextOut(this->x, this->y, multiByteCharacter->GetContents().c_str());

	//2. ������ �ʺ�ŭ �����ġ�� �̵��ϴ�.
	this->x += this->textEditor->characterMetrics->GetWidth(128); // ��Ƽ����Ʈ������ ���̰� 128��°�� ����Ǿ� �ֽ��ϴ�.
}