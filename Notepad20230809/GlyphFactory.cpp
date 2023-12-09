//GlyphFactory.cpp

#include "GlyphFactory.h"
#include "Paper.h"
#include "Row.h"
#include "DummyRow.h"
#include "SingleByteCharacter.h"
#include "MultiByteCharacter.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <new>
#include <memory>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

/*
클래스명: GlyphFactory
기    능: 입력된 문자에 알맞는 클래스(Concrete Class)의 객체를 생성하다. (관련 디자인패턴: Factory method)
*/

GlyphFactory::GlyphFactory() {
}

GlyphFactory::~GlyphFactory() {

}

/*
함수명칭: Create
기    능: 입력된 문자에 알맞는 클래스(Concrete Class)의 객체를 생성하다.
입    력: 문자열
출    력: Glyph Link
*/
Glyph* GlyphFactory::Create(char(*contents)) {
	Glyph* glyph = 0;
	
	//1. NULL 문자가 입력되면 종이를 생성하다.
	if (contents[0] == '\0') {
		glyph = new Paper;
	}

	//2. '\r\n' 문자가 입력되면 줄을 생성하다.
	else if (contents[0] == '\r' && contents[1] == '\n') {
		glyph = new Row;
	}
	
	//3. '\r' 문자가 입력되면 Dummy 줄을 생성하다.
	else if (contents[0] == '\r') {
		glyph = new DummyRow;
	}

	//4. singlebyte 문자가 입력되면 SingleByteCharacter를 생성하다.
	else if (contents[0] >= 0 && contents[0] < ASCII) {
		glyph = new SingleByteCharacter(contents[0]);
	}

	//5. multiByte 문자가 입력되면 MultiByteCharacter를 생성하다.
	else if (!(contents[0] >= 0 && contents[0] < ASCII)) {
		glyph = new MultiByteCharacter(contents);
	}

	return glyph;
}