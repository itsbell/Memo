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
Ŭ������: CharacterMetrics
��    ��: ������ ���̸� ���ϴ�.
*/

CharacterMetrics::CharacterMetrics() {

}

/*
�Լ��̸�: CharacterMetrics
��    ��: ������ �ʺ�� ���̸� ���ϴ�.
��    ��: CDC Link, Font Link
��    ��: ����
*/
CharacterMetrics::CharacterMetrics(CDC* dc, Font* font, ZoomController* zoomController) {
	Long i;
	char ascii;
	SIZE psizl;
	TEXTMETRIC lptm = { 0, };
	
	// ��Ʈ����
	CFont cFont;

	// CreateFont �Ű�����
	cFont.CreateFont(
		font->GetLogfont().lfHeight * zoomController->GetMagnification(),			//���ڳ���
		font->GetLogfont().lfWidth,				//���ڳʺ�
		font->GetLogfont().lfEscapement,		//��°���
		font->GetLogfont().lfOrientation,		//���� �������� ����
		font->GetLogfont().lfWeight,			//���ڱ���
		font->GetLogfont().lfItalic,			//Italic ���뿩��
		font->GetLogfont().lfUnderline,			//�������뿩��
		font->GetLogfont().lfStrikeOut,			//��Ҽ����뿩��
		font->GetLogfont().lfCharSet,			//���ڼ�����
		font->GetLogfont().lfOutPrecision,		//������е�
		font->GetLogfont().lfClipPrecision,		//Ŭ�������е�
		font->GetLogfont().lfQuality,			//��¹���ǰ��
		font->GetLogfont().lfPitchAndFamily,	//�۲�Pitch
		font->GetLogfont().lfFaceName			//�۲�
	);

	// DC��CFont Object ����
	dc->SelectObject(cFont);

	if (font->GetLogfont().lfItalic == '\0') {
		//1. �ƽ�Ű�ڵ� �� ��ŭ �ݺ��ϴ�.
		for (i = 0; i < ASCII; i++) {
			ascii = i;
			//1.1. �ش� �ƽ�Ű�ڵ��� �ʺ� ���ϴ�.
			::GetTextExtentPoint32(*dc, &ascii, 1, &psizl);
			this->widths[i] = psizl.cx;
		}
	}
	else {
		for (i = 0; i < ASCII; i++) {
			ascii = i;
			//1.1. �ش� �ƽ�Ű�ڵ��� �ʺ� ���ϴ�.
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

	//2. TAB�� �ʺ� ��ġ��.
	this->widths[TAB] = this->widths[SPACE] * 8;

	//3. ���Ÿ�ü�� �ƴϸ� MultiByte������ �ʺ� ���� �迭�� �����ϴ�. (������ �ѱ� "��" ���)
	::GetTextExtentPoint32(*dc, "��", 2, &psizl);

	if (font->GetLogfont().lfItalic == '\0') {
		this->widths[i] = psizl.cx;
	}

	//4. ���Ÿ�ü�� MultiByte������ �ʺ� ���� 1.5�� ũ�� �迭�� �����ϴ�. (������ �ѱ� "��" ���)
	else {
		this->widths[i] = psizl.cx * 1.5F;
	}

	//5. ������ ���̸� ���� �����ϴ�.
	::GetTextMetrics(*dc, &lptm);
	this->height = lptm.tmHeight;

	//6. CFont Object ����
	cFont.DeleteObject();
}

CharacterMetrics::CharacterMetrics(HDC hDC, Font* font) {
	Long i;
	char ascii;
	SIZE psizl;
	TEXTMETRIC lptm = { 0, };

	CFont cFont;
	cFont.CreateFont(
		font->GetLogfont().lfHeight * 8.3333333333333333333,			//���ڳ��� DPI 600����
		font->GetLogfont().lfWidth,				//���ڳʺ�
		font->GetLogfont().lfEscapement,		//��°���
		font->GetLogfont().lfOrientation,		//���� �������� ����
		font->GetLogfont().lfWeight,			//���ڱ���
		font->GetLogfont().lfItalic,			//Italic ���뿩��
		font->GetLogfont().lfUnderline,			//�������뿩��
		font->GetLogfont().lfStrikeOut,			//��Ҽ����뿩��
		font->GetLogfont().lfCharSet,			//���ڼ�����
		font->GetLogfont().lfOutPrecision,		//������е�
		font->GetLogfont().lfClipPrecision,		//Ŭ�������е�
		font->GetLogfont().lfQuality,			//��¹���ǰ��
		font->GetLogfont().lfPitchAndFamily,	//�۲�Pitch
		font->GetLogfont().lfFaceName			//�۲�
	);
	SelectObject(hDC, cFont);

	//1. �ƽ�Ű�ڵ� �� ��ŭ �ݺ��ϴ�.
	for (i = 0; i < ASCII; i++) {
		ascii = i;

		//1.1. �ش� �ƽ�Ű�ڵ��� �ʺ� ���ϴ�.
		::GetTextExtentPoint32(hDC, &ascii, 1, &psizl);

		//1.2. ���Ÿ�ü�� �ƴϸ� �ʺ� �迭�� �����ϴ�.
		if (font->GetLogfont().lfItalic == '\0') {
			this->widths[i] = psizl.cx;
		}

		//1.3. ���Ÿ�ü�� ���� �ʺ񿡼� 1.5�� ũ�� �迭�� �����ϴ�.
		else {
			this->widths[i] = psizl.cx * 1.5F;
		}
	}

	//2. TAB�� �ʺ� ��ġ��.
	this->widths[TAB] = this->widths[SPACE] * 8;

	//3. ���Ÿ�ü�� �ƴϸ� MultiByte������ �ʺ� ���� �迭�� �����ϴ�. (������ �ѱ� "��" ���)
	::GetTextExtentPoint32(hDC, "��", 2, &psizl);

	if (font->GetLogfont().lfItalic == '\0') {
		this->widths[i] = psizl.cx;
	}

	//4. ���Ÿ�ü�� MultiByte������ �ʺ� ���� 1.5�� ũ�� �迭�� �����ϴ�. (������ �ѱ� "��" ���)
	else {
		this->widths[i] = psizl.cx * 1.5F;
	}

	//5. ������ ���̸� ���� �����ϴ�.
	::GetTextMetrics(hDC, &lptm);
	this->height = lptm.tmHeight;

	//6. CFont Object ����
	cFont.DeleteObject();

}

CharacterMetrics::~CharacterMetrics() {

}