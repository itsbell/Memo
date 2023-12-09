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
클래스명: TextOutVisitor
기    능: TextOutVisitor -▷ Visitor / 텍스트를 전체 출력하는 클래스 / 관련 디자인패턴(Visitor 디자인패턴)
*/

/*
함수이름: TextOutVisitor
기    능: TextOutVisitor 매개변수를 가지는 생성자 (CDC클래스의 TextOut함수를 쓰기 위해 CDC*를 매개변수로 가진다)
입    력: CDC Link
출    력: 없음
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
함수이름: Visit
기    능: 종이 내용을 화면에 출력하다.
입    력: 종이
출    력: 없음
*/
void TextOutVisitor::Visit(Paper* paper) {
	Long i = this->start;
	Glyph* row;

	while (i < this->end && i < paper->GetLength()) {
		row = paper->GetAt(i);
		//1.1. 줄 내용을 모두 출력하다.
		row->Accept(this);
		//1.3. Y좌표를 문자의 높이 만큼 이동하다.
		this->y += this->textEditor->characterMetrics->GetHeight();
		i++;
	}
}

/*
함수이름: Visit
기    능: 줄 내용을 화면에 출력하다.
입    력: 줄
출    력: 없음
*/
void TextOutVisitor::Visit(Row* row) {
	Long i = 0;
	Long sum = 0;
	Glyph* character = 0;
	
	//1. 문자 수 만큼 반복하다.
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
함수이름: Visit
기    능: 문자 내용을 화면에 출력하다.
입    력: 문자
출    력: 없음
*/
void TextOutVisitor::Visit(SingleByteCharacter* singleByteCharacter) {
 //원본
	//1. 문자를 출력하다.
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

	//2. 문자의 너비만큼 출력위치를 이동하다.
	this->x += this->textEditor->characterMetrics->GetWidth(singleByteCharacter->GetCharacter());


#if 0 // DrawText (겹치진 않으나 일부가 잘림)
	//1. 문자를 출력하다.
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
	
	//2. 문자의 너비만큼 출력위치를 이동하다.
	this->x += this->textEditor->characterMetrics->GetWidth(singleByteCharacter->GetCharacter());
#endif
}

/*
함수이름: Visit
기    능: 문자 내용을 화면에 출력하다.
입    력: 문자
출    력: 없음
*/
void TextOutVisitor::Visit(MultiByteCharacter* multiByteCharacter) {

	//1. 문자를 출력하다.
	this->dc->TextOut(this->x, this->y, multiByteCharacter->GetContents().c_str());

	//2. 문자의 너비만큼 출력위치를 이동하다.
	this->x += this->textEditor->characterMetrics->GetWidth(128); // 멀티바이트문자의 넓이가 128번째에 저장되어 있습니다.
}