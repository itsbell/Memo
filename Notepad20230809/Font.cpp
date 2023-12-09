//Font.cpp
#include "Font.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
클래스명: Font
기    능: 폰트 정보를 담고있는 클래스
*/

Font::Font() {

}

Font::Font(LOGFONT lf) {
	this->lf = lf;
}

Font::~Font() {

}