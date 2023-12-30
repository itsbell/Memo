//Font.cpp
#include "Font.h"

#pragma warning (disable:4996)

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
	//�۲� �ʱⰪ
	this->lf.lfHeight = -26;
	this->lf.lfWidth = 0;
	this->lf.lfEscapement = 0;
	this->lf.lfOrientation = 0;
	this->lf.lfWeight = 700;
	this->lf.lfItalic = FALSE;
	this->lf.lfUnderline = FALSE;
	this->lf.lfStrikeOut = FALSE;
	this->lf.lfCharSet = DEFAULT_CHARSET;
	this->lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	this->lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	this->lf.lfQuality = DEFAULT_QUALITY;
	this->lf.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy(this->lf.lfFaceName, _T("���� ���"));
}

Font::Font(LOGFONT lf) {
	this->lf = lf;
}

Font::~Font() {

}