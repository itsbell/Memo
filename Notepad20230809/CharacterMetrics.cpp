//CharacterMetrics.cpp
#include "CharacterMetrics.h"
#include "Font.h"
#include "ZoomController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//git test
/*
클래스명: CharacterMetrics
기    능: 문자의 길이를 구하다.
*/

CharacterMetrics::CharacterMetrics() {

}

/*
함수이름: CharacterMetrics
기    능: 문자의 너비와 높이를 구하다.
입    력: CDC Link, Font Link
출    력: 없음
*/
CharacterMetrics::CharacterMetrics(CDC* dc, Font* font, ZoomController* zoomController) {
	Long i;
	char ascii;
	SIZE psizl;
	TEXTMETRIC lptm = { 0, };
	
	// 폰트생성
	CFont cFont;

	// CreateFont 매개변수
	cFont.CreateFont(
		font->GetLogfont().lfHeight * zoomController->GetMagnification(),			//글자높이
		font->GetLogfont().lfWidth,				//글자너비
		font->GetLogfont().lfEscapement,		//출력각도
		font->GetLogfont().lfOrientation,		//기준 선에서의 각도
		font->GetLogfont().lfWeight,			//글자굵기
		font->GetLogfont().lfItalic,			//Italic 적용여부
		font->GetLogfont().lfUnderline,			//밑줄적용여부
		font->GetLogfont().lfStrikeOut,			//취소선적용여부
		font->GetLogfont().lfCharSet,			//문자셋종류
		font->GetLogfont().lfOutPrecision,		//출력정밀도
		font->GetLogfont().lfClipPrecision,		//클리핑정밀도
		font->GetLogfont().lfQuality,			//출력문자품질
		font->GetLogfont().lfPitchAndFamily,	//글꼴Pitch
		font->GetLogfont().lfFaceName			//글꼴
	);

	// DC에CFont Object 설정
	dc->SelectObject(cFont);

	if (font->GetLogfont().lfItalic == '\0') {
		//1. 아스키코드 수 만큼 반복하다.
		for (i = 0; i < ASCII; i++) {
			ascii = i;
			//1.1. 해당 아스키코드의 너비를 구하다.
			::GetTextExtentPoint32(*dc, &ascii, 1, &psizl);
			this->widths[i] = psizl.cx;
		}
	}
	else {
		for (i = 0; i < ASCII; i++) {
			ascii = i;
			//1.1. 해당 아스키코드의 너비를 구하다.
			::GetTextExtentPoint32(*dc, &ascii, 1, &psizl);
			if (i == 'i') {
				this->widths[i] = psizl.cx * 2;
			}
			else if (i == 'j') {
				this->widths[i] = psizl.cx * 2.3F;
			}
			else if (i == 'l') {
				this->widths[i] = psizl.cx * 2;
			}
			else {
				this->widths[i] = psizl.cx * 1.5F;
			}
		}
	}

	//2. TAB의 너비를 고치다.
	this->widths[TAB] = this->widths[SPACE] * 8;

	//3. 이탤릭체가 아니면 MultiByte문자의 너비를 구해 배열에 저장하다. (임의의 한글 "가" 사용)
	::GetTextExtentPoint32(*dc, "가", 2, &psizl);

	if (font->GetLogfont().lfItalic == '\0') {
		this->widths[i] = psizl.cx;
	}

	//4. 이탤릭체면 MultiByte문자의 너비를 구해 1.5배 크게 배열에 저장하다. (임의의 한글 "가" 사용)
	else {
		this->widths[i] = psizl.cx * 1.5F;
	}

	//5. 문자의 높이를 구해 저장하다.
	::GetTextMetrics(*dc, &lptm);
	this->height = lptm.tmHeight;

	//6. CFont Object 제거
	cFont.DeleteObject();
}

CharacterMetrics::CharacterMetrics(HDC hDC, Font* font) {
	Long i;
	char ascii;
	SIZE psizl;
	TEXTMETRIC lptm = { 0, };

	CFont cFont;
	cFont.CreateFont(
		font->GetLogfont().lfHeight * 8.3333333333333333333,			//글자높이 DPI 600기준
		font->GetLogfont().lfWidth,				//글자너비
		font->GetLogfont().lfEscapement,		//출력각도
		font->GetLogfont().lfOrientation,		//기준 선에서의 각도
		font->GetLogfont().lfWeight,			//글자굵기
		font->GetLogfont().lfItalic,			//Italic 적용여부
		font->GetLogfont().lfUnderline,			//밑줄적용여부
		font->GetLogfont().lfStrikeOut,			//취소선적용여부
		font->GetLogfont().lfCharSet,			//문자셋종류
		font->GetLogfont().lfOutPrecision,		//출력정밀도
		font->GetLogfont().lfClipPrecision,		//클리핑정밀도
		font->GetLogfont().lfQuality,			//출력문자품질
		font->GetLogfont().lfPitchAndFamily,	//글꼴Pitch
		font->GetLogfont().lfFaceName			//글꼴
	);
	SelectObject(hDC, cFont);

	//1. 아스키코드 수 만큼 반복하다.
	for (i = 0; i < ASCII; i++) {
		ascii = i;

		//1.1. 해당 아스키코드의 너비를 구하다.
		::GetTextExtentPoint32(hDC, &ascii, 1, &psizl);

		//1.2. 이탤릭체가 아니면 너비를 배열에 저장하다.
		if (font->GetLogfont().lfItalic == '\0') {
			this->widths[i] = psizl.cx;
		}

		//1.3. 이탤릭체면 구한 너비에서 1.5배 크게 배열에 저장하다.
		else {
			this->widths[i] = psizl.cx * 1.5F;
		}
	}

	//2. TAB의 너비를 고치다.
	this->widths[TAB] = this->widths[SPACE] * 8;

	//3. 이탤릭체가 아니면 MultiByte문자의 너비를 구해 배열에 저장하다. (임의의 한글 "가" 사용)
	::GetTextExtentPoint32(hDC, "가", 2, &psizl);

	if (font->GetLogfont().lfItalic == '\0') {
		this->widths[i] = psizl.cx;
	}

	//4. 이탤릭체면 MultiByte문자의 너비를 구해 1.5배 크게 배열에 저장하다. (임의의 한글 "가" 사용)
	else {
		this->widths[i] = psizl.cx * 1.5F;
	}

	//5. 문자의 높이를 구해 저장하다.
	::GetTextMetrics(hDC, &lptm);
	this->height = lptm.tmHeight;

	//6. CFont Object 제거
	cFont.DeleteObject();

}

CharacterMetrics::~CharacterMetrics() {

}