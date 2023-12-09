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
Ŭ������: GlyphFactory
��    ��: �Էµ� ���ڿ� �˸´� Ŭ����(Concrete Class)�� ��ü�� �����ϴ�. (���� ����������: Factory method)
*/

GlyphFactory::GlyphFactory() {
}

GlyphFactory::~GlyphFactory() {

}

/*
�Լ���Ī: Create
��    ��: �Էµ� ���ڿ� �˸´� Ŭ����(Concrete Class)�� ��ü�� �����ϴ�.
��    ��: ���ڿ�
��    ��: Glyph Link
*/
Glyph* GlyphFactory::Create(char(*contents)) {
	Glyph* glyph = 0;
	
	//1. NULL ���ڰ� �ԷµǸ� ���̸� �����ϴ�.
	if (contents[0] == '\0') {
		glyph = new Paper;
	}

	//2. '\r\n' ���ڰ� �ԷµǸ� ���� �����ϴ�.
	else if (contents[0] == '\r' && contents[1] == '\n') {
		glyph = new Row;
	}
	
	//3. '\r' ���ڰ� �ԷµǸ� Dummy ���� �����ϴ�.
	else if (contents[0] == '\r') {
		glyph = new DummyRow;
	}

	//4. singlebyte ���ڰ� �ԷµǸ� SingleByteCharacter�� �����ϴ�.
	else if (contents[0] >= 0 && contents[0] < ASCII) {
		glyph = new SingleByteCharacter(contents[0]);
	}

	//5. multiByte ���ڰ� �ԷµǸ� MultiByteCharacter�� �����ϴ�.
	else if (!(contents[0] >= 0 && contents[0] < ASCII)) {
		glyph = new MultiByteCharacter(contents);
	}

	return glyph;
}