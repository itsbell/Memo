//Preview.cpp
#include "Preview.h"
#include "Notepad.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "CharacterMetrics.h"
#include "Document.h"
#include "Font.h"
#include "ZoomController.h"
#include "GlyphFactory.h"
#include "PreviewTextOutVisitor.h"
#include "DummyRow.h"
#include "FileMaker.h"
#include "resource.h"
#include <afxext.h>

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(Preview, CFrameWnd)

	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(IDC_BUTTON_PREVIOUS,OnPreviousButtonClicked)
	ON_COMMAND(IDC_BUTTON_NEXT, OnNextButtonClicked)
	ON_COMMAND(IDC_BUTTON_PRINT, OnPrintButtonClicked)

END_MESSAGE_MAP()

Preview::Preview(TextEditor* textEditor, Long capacity)
	:notes(capacity){
	this->textEditor = textEditor;
	this->capacity = capacity;
	this->length = 0;
	this->currentPage = 0;
	this->paperX = 0;
	this->paperY = 0;
	this->zoomController = 0;
	this->characterMetrics = 0;
	this->document = textEditor->document;
	this->note = 0;
	this->current = 0;
	this->rectPaper = { 0, };
	this->rectMargin = { 0, };
	this->x = 0;
	this->y = 0;
	this->previous = 0;
	this->next = 0;
	this->print = 0;
}

Preview::~Preview() {
	Long i = 0;

	this->textEditor->GetParent()->ShowWindow(SW_SHOW);	
	
	while (i < this->length) {
		if (this->notes.GetAt(i) != 0) {
			delete this->notes.GetAt(i);
		}
		i++;
	}
	
	if (this->characterMetrics != 0) {
		delete this->characterMetrics;
	}
	if (this->zoomController != 0) {
		delete this->zoomController;
	}
	if (this->previous != 0) {
		delete this->previous;
	}
	if (this->next != 0) {
		delete this->next;
	}
	if (this->print != 0) {
		delete this->print;
	}
}

int Preview::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	Long index;
	GlyphFactory glyphFactory;

	//1. 이전 버튼을 만들다.
	this->previous = new CBitmapButton;
	this->previous->Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(0, 0, 40, 40), this, IDC_BUTTON_PREVIOUS);
	dynamic_cast<CBitmapButton*>(this->previous)->LoadBitmaps(IDB_BITMAP_PREVIOUS, NULL, NULL, NULL);
	dynamic_cast<CBitmapButton*>(this->previous)->SizeToContent();

	//2. 다음 버튼을 만들다.
	this->next = new CBitmapButton;
	this->next->Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(40, 0, 80, 40), this, IDC_BUTTON_NEXT);
	dynamic_cast<CBitmapButton*>(this->next)->LoadBitmaps(IDB_BITMAP_NEXT, NULL, NULL, NULL);
	dynamic_cast<CBitmapButton*>(this->next)->SizeToContent();

	//3. 인쇄 버튼을 만들다.
	this->print = new CBitmapButton;
	this->print->Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(80, 0, 120, 40), this, IDC_BUTTON_PRINT);
	dynamic_cast<CBitmapButton*>(this->print)->LoadBitmaps(IDB_BITMAP_PRINT, NULL, NULL, NULL);
	dynamic_cast<CBitmapButton*>(this->print)->SizeToContent();

	//3. ZoomController을 만든다.
	this->zoomController = new ZoomController;

	this->paperX = this->textEditor->psd.ptPaperSize.x / 100 * (72 / 25.4F); //DPI:72 기준 pixels 공식
	this->paperY = this->textEditor->psd.ptPaperSize.y / 100 * (72 / 25.4F);

	this->rectPaper.left = -paperX / 2;
	this->rectPaper.top = 0;
	if (this->paperX % 2 != 0) {
		this->rectPaper.right = this->paperX / 2 + 1;
	}
	else {
		this->rectPaper.right = this->paperX / 2;
	}
	this->rectPaper.bottom = this->paperY;

	//Margin DPI 72 기준 mm convert to pixels
	this->rectMargin.left = this->rectPaper.left + this->textEditor->psd.rtMargin.left / 100 * (72 / 25.4F);
	this->rectMargin.top = this->rectPaper.top + this->textEditor->psd.rtMargin.top / 100 * (72 / 25.4F);
	this->rectMargin.right = this->rectPaper.right - this->textEditor->psd.rtMargin.right / 100 * (72 / 25.4F);
	this->rectMargin.bottom = this->rectPaper.bottom - this->textEditor->psd.rtMargin.bottom / 100 * (72 / 25.4F);

	//CharacterMetrics
	this->characterMetrics = new CharacterMetrics(this->GetDC(), this->textEditor->font, this->zoomController);

	//Paper
	index = this->Add(glyphFactory.Create((char*)"\0"));
	this->note = this->notes.GetAt(index);

	//PreviewNotes를 만들다.
	this->MakePreviewNotes();
	this->currentPage = 1;

	return 0;
}

void Preview::OnPaint() {

	CPaintDC dc(this);

	CRect client;
	this->GetClientRect(&client);

	//1. 배경색을 색칠하다.
	CPen pen;
	CPen* pOldPen;
	CBrush brush;
	CBrush* pOldBrush;

	pen.CreatePen(PS_SOLID, 1, 0x00ffffff); //하얀색 펜 설정
	pOldPen = dc.SelectObject(&pen);
	brush.CreateSolidBrush(0xcccccc); //회색 채우기 색 설정
	pOldBrush = dc.SelectObject(&brush);
	dc.Rectangle(0, 40, client.right, client.bottom); // 사각형 그리기
	dc.SelectObject(pOldBrush);

	pen.DeleteObject();
	brush.DeleteObject();

	//2. 용지를 그리다.
	//dc.SetMapMode(MM_HIMETRIC);
	dc.SetViewportOrg(client.Width() / 2, 50);

	pen.CreatePen(PS_SOLID, 1, 0x00000001);
	pOldPen = dc.SelectObject(&pen);

	brush.CreateSolidBrush(0x00ffffff);
	pOldBrush = dc.SelectObject(&brush);

	CRect rectPaper(
		this->rectPaper.left * this->zoomController->GetMagnification(), this->rectPaper.top * this->zoomController->GetMagnification(),
		this->rectPaper.right * this->zoomController->GetMagnification(), this->rectPaper.bottom * this->zoomController->GetMagnification()
	);

	CRect rectMargin(
		this->rectMargin.left * this->zoomController->GetMagnification(), this->rectMargin.top * this->zoomController->GetMagnification(),
		this->rectMargin.right * this->zoomController->GetMagnification(), this->rectMargin.bottom * this->zoomController->GetMagnification()
	);

	dc.Rectangle(&rectPaper);
	dc.SelectObject(pOldBrush);

	pen.DeleteObject();
	brush.DeleteObject();

	//2. CFont 클래스 생성
	CFont cFont;

	//3. 폰트를 만들다.
	cFont.CreateFont(
		this->textEditor->font->GetLogfont().lfHeight * this->zoomController->GetMagnification(),			//글자높이
		this->textEditor->font->GetLogfont().lfWidth,				//글자너비
		this->textEditor->font->GetLogfont().lfEscapement,		//출력각도
		this->textEditor->font->GetLogfont().lfOrientation,		//기준 선에서의 각도
		this->textEditor->font->GetLogfont().lfWeight,			//글자굵기
		this->textEditor->font->GetLogfont().lfItalic,			//Italic 적용여부
		this->textEditor->font->GetLogfont().lfUnderline,			//밑줄적용여부
		this->textEditor->font->GetLogfont().lfStrikeOut,			//취소선적용여부
		this->textEditor->font->GetLogfont().lfCharSet,			//문자셋종류
		this->textEditor->font->GetLogfont().lfOutPrecision,		//출력정밀도
		this->textEditor->font->GetLogfont().lfClipPrecision,		//클리핑정밀도
		this->textEditor->font->GetLogfont().lfQuality,			//출력문자품질
		this->textEditor->font->GetLogfont().lfPitchAndFamily,	//글꼴Pitch
		this->textEditor->font->GetLogfont().lfFaceName			//글꼴
	);

	//4. DC에 CFont Object 설정
	dc.SelectObject(cFont);

	GlyphVisitor* previewTextOutVisitor = new PreviewTextOutVisitor(this, &dc, rectMargin.left, rectMargin.top);

	Glyph* note = this->notes.GetAt(currentPage - 1);
	note->Accept(previewTextOutVisitor);

	cFont.DeleteObject();

	if (previewTextOutVisitor != 0) {
		delete previewTextOutVisitor;
	}

	// 여백 점선을 그리다.
	pen.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
	pOldPen = dc.SelectObject(&pen);

	dc.MoveTo(rectMargin.left, rectPaper.top);
	dc.LineTo(rectMargin.left, rectPaper.bottom);

	dc.MoveTo(rectPaper.left, rectMargin.top);
	dc.LineTo(rectPaper.right, rectMargin.top);

	dc.MoveTo(rectMargin.right, rectPaper.top);
	dc.LineTo(rectMargin.right, rectPaper.bottom);

	dc.MoveTo(rectPaper.left, rectMargin.bottom);
	dc.LineTo(rectPaper.right, rectMargin.bottom);

	dc.SelectObject(pOldPen);

	pen.DeleteObject();

	cFont.DeleteObject();
	//4. DC에 CFont Object 설정
	cFont.CreateFont(
		-12,			//글자높이
		0,				//글자너비
		0,				//출력각도
		0,				//기준 선에서의 각도
		700,			//글자굵기
		'\0',			//Italic 적용여부
		'\0',			//밑줄적용여부
		'\0',			//취소선적용여부
		'\x1',			//문자셋종류
		'\0',			//출력정밀도
		'\0',			//클리핑정밀도
		'\0',			//출력문자품질
		'\0',			//글꼴Pitch
		"맑은고딕"		//글꼴
	);
	dc.SelectObject(cFont);
	CString information;
	information.Format("(%d쪽 / %d쪽) 용지: A4 [210 x 297 mm]", this->currentPage, this->GetLength());
	dc.TextOut(70, rectPaper.bottom, information);
	cFont.DeleteObject();
}

Long Preview::Add(Glyph* paper) {
	Long index;

	if (this->notes.GetLength() < this->notes.GetCapacity()) {
		index = this->notes.Store(this->notes.GetLength(), paper);
	}

	else {
		index = this->notes.AppendFromRear(paper);
		this->capacity++;
	}

	this->length++;

	return index;
}

void Preview::MakePreviewNotes() {
	char buffer[MAX];
	char material[2];
	Long i;
	Long j;
	Long x = 0;
	Long y = 0;
	Long index;
	Long byte;
	FILE* file;
	LONG paperWidth = this->paperX - this->textEditor->psd.rtMargin.left / 100 * (72 / 25.4F) - this->textEditor->psd.rtMargin.right / 100 * (72 / 25.4F); // 여백을 제외한 종이 폭
	LONG paperHeight = this->paperY - this->textEditor->psd.rtMargin.top / 100 * (72 / 25.4F) - this->textEditor->psd.rtMargin.bottom / 100 * (72 / 25.4F); // 여백을 제외한 종이 높이
	Glyph* row;
	Glyph* character;
	GlyphFactory glyphFactory;
	FileMaker fm(this->textEditor);

	file = fopen(fm.GetFile(), "rt");
	if (file != NULL) {
		i = 1;
		while (i < this->document->GetStart()) {
			if (y + this->characterMetrics->GetHeight() > paperHeight) {
				index = this->Add(glyphFactory.Create((char*)"\0"));
				y = 0;
				this->note = this->notes.GetAt(index);
			}
			index = this->note->Add(glyphFactory.Create((char*)"\r\n"));
			y += this->characterMetrics->GetHeight();
			this->current = this->note->GetAt(index);
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			buffer[byte] = '\0';
			j = 0;
			while (j < byte) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					material[0] = buffer[j];
					material[1] = '\0';
				}
				else {
					material[0] = buffer[j];
					j++;
					material[1] = buffer[j];
				}
				character = glyphFactory.Create(material);
				if (this->current->GetRowLength() + character->GetWidth(this->characterMetrics) > paperWidth) {
					if (y + this->characterMetrics->GetHeight() > paperHeight) {
						index = this->Add(glyphFactory.Create((char*)"\0"));
						y = 0;
						this->note = this->notes.GetAt(index);
					}
					index = this->note->Add(glyphFactory.Create((char*)"\r\n"));
					y += this->characterMetrics->GetHeight();
					this->current = this->note->GetAt(index);
				}
				this->current->Add(character, this->characterMetrics);
				j++;
			}
			i++;
		}
		
		//1. textEditor의 종이에서 줄 수 만큼 반복하다.
		i = 1;
		while (i <= this->textEditor->note->GetLength()) {
			//1.1. 줄을 구하다.
			row = this->textEditor->note->GetAt(i - 1);
			//1.2. preview에서 현재 Y축 + 문자높이가 종이높이 보다 크면
			if (y + this->characterMetrics->GetHeight() > paperHeight) {
				//1.2.1. 종이를 만들어 preview의 notes에 추가하다.
				index = this->Add(glyphFactory.Create((char*)"\0"));
				y = 0;
				this->note = this->notes.GetAt(index);
			}
			//1.3. 줄을 추가하다.
			if (!dynamic_cast<DummyRow*>(row)) {
				index = this->note->Add(glyphFactory.Create((char*)"\r\n"));
				y += this->characterMetrics->GetHeight();
				this->current = this->note->GetAt(index);
			}
			//1.4. 줄의 문자수 만큼 반복하다.
			j = 0;
			while (j < row->GetLength()) {
				//1.4.1. 문자를 구하다.
				character = row->GetAt(j);
				//1.4.2. 줄의 X축 좌표 + 문자 폭이 종이의 폭보다 크면
				if (this->current->GetRowLength() + character->GetWidth(this->characterMetrics) > paperWidth) {
					//1.4.2.1. preview에서 현재 Y축 + 문자높이가 종이높이 보다 크면
					if (y + this->characterMetrics->GetHeight() > paperHeight) {
						//1.4.2.1.1. 종이를 만들어 preview의 notes에 추가하다.
						index = this->Add(glyphFactory.Create((char*)"\0"));
						y = 0;
						this->note = this->notes.GetAt(index);
					}
					//1.4.2.2. 종이에서 줄을 만들어 추가하다.
					index = this->note->Add(glyphFactory.Create((char*)"\r\n"));
					y += this->characterMetrics->GetHeight();
					this->current = this->note->GetAt(index);
				}
				//1.4.3. 복제한 문자를 추가하다.
				this->current->Add(character->Clone(), this->characterMetrics);
				x += character->GetWidth(this->characterMetrics);
				j++;
			}
			i++;
		}

		i = this->document->GetEnd() + 1;
		while (i <= this->document->GetLength()) {
			if (y + this->characterMetrics->GetHeight() > paperHeight) {
				index = this->Add(glyphFactory.Create((char*)"\0"));
				y = 0;
				this->note = this->notes.GetAt(index);
			}
			index = this->note->Add(glyphFactory.Create((char*)"\r\n"));
			y += this->characterMetrics->GetHeight();
			this->current = this->note->GetAt(index);
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			buffer[byte] = '\0';
			j = 0;
			while (j < byte) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					material[0] = buffer[j];
					material[1] = '\0';
				}
				else {
					material[0] = buffer[j];
					j++;
					material[1] = buffer[j];
				}
				character = glyphFactory.Create(material);
				if (this->current->GetRowLength() + character->GetWidth(this->characterMetrics) > paperWidth) {
					if (y + this->characterMetrics->GetHeight() > paperHeight) {
						index = this->Add(glyphFactory.Create((char*)"\0"));
						y = 0;
						this->note = this->notes.GetAt(index);
					}
					index = this->note->Add(glyphFactory.Create((char*)"\r\n"));
					y += this->characterMetrics->GetHeight();
					this->current = this->note->GetAt(index);
				}
				this->current->Add(character, this->characterMetrics);
				j++;
			}
			i++;
		}
		fclose(file);
	}
}


void Preview::OnPreviousButtonClicked() {

	this->currentPage--;
	if (this->currentPage <= 0) {
		this->currentPage++;
	}
	this->Invalidate(FALSE);
}

void Preview::OnNextButtonClicked() {
	
	this->currentPage++;
	if (this->currentPage > this->length) {
		this->currentPage--;
	}
	this->Invalidate(FALSE);
}

void Preview::OnPrintButtonClicked() {
	this->textEditor->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_PRINT, NULL));
	this->textEditor->GetParent()->ShowWindow(SW_HIDE);
}

Glyph* Preview::GetAt(Long i) {
	return this->notes.GetAt(i);
}