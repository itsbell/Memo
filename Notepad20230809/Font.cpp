//Font.cpp
#include "Font.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Ŭ������: Font
��    ��: ��Ʈ ������ ����ִ� Ŭ����
*/

Font::Font() {

}

Font::Font(LOGFONT lf) {
	this->lf = lf;
}

Font::~Font() {

}