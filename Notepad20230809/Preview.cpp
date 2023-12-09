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

	//1. ���� ��ư�� �����.
	this->previous = new CBitmapButton;
	this->previous->Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(0, 0, 40, 40), this, IDC_BUTTON_PREVIOUS);
	dynamic_cast<CBitmapButton*>(this->previous)->LoadBitmaps(IDB_BITMAP_PREVIOUS, NULL, NULL, NULL);
	dynamic_cast<CBitmapButton*>(this->previous)->SizeToContent();

	//2. ���� ��ư�� �����.
	this->next = new CBitmapButton;
	this->next->Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(40, 0, 80, 40), this, IDC_BUTTON_NEXT);
	dynamic_cast<CBitmapButton*>(this->next)->LoadBitmaps(IDB_BITMAP_NEXT, NULL, NULL, NULL);
	dynamic_cast<CBitmapButton*>(this->next)->SizeToContent();

	//3. �μ� ��ư�� �����.
	this->print = new CBitmapButton;
	this->print->Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(80, 0, 120, 40), this, IDC_BUTTON_PRINT);
	dynamic_cast<CBitmapButton*>(this->print)->LoadBitmaps(IDB_BITMAP_PRINT, NULL, NULL, NULL);
	dynamic_cast<CBitmapButton*>(this->print)->SizeToContent();

	//3. ZoomController�� �����.
	this->zoomController = new ZoomController;

	this->paperX = this->textEditor->psd.ptPaperSize.x / 100 * (72 / 25.4F); //DPI:72 ���� pixels ����
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

	//Margin DPI 72 ���� mm convert to pixels
	this->rectMargin.left = this->rectPaper.left + this->textEditor->psd.rtMargin.left / 100 * (72 / 25.4F);
	this->rectMargin.top = this->rectPaper.top + this->textEditor->psd.rtMargin.top / 100 * (72 / 25.4F);
	this->rectMargin.right = this->rectPaper.right - this->textEditor->psd.rtMargin.right / 100 * (72 / 25.4F);
	this->rectMargin.bottom = this->rectPaper.bottom - this->textEditor->psd.rtMargin.bottom / 100 * (72 / 25.4F);

	//CharacterMetrics
	this->characterMetrics = new CharacterMetrics(this->GetDC(), this->textEditor->font, this->zoomController);

	//Paper
	index = this->Add(glyphFactory.Create((char*)"\0"));
	this->note = this->notes.GetAt(index);

	//PreviewNotes�� �����.
	this->MakePreviewNotes();
	this->currentPage = 1;

	return 0;
}

void Preview::OnPaint() {

	CPaintDC dc(this);

	CRect client;
	this->GetClientRect(&client);

	//1. ������ ��ĥ�ϴ�.
	CPen pen;
	CPen* pOldPen;
	CBrush brush;
	CBrush* pOldBrush;

	pen.CreatePen(PS_SOLID, 1, 0x00ffffff); //�Ͼ�� �� ����
	pOldPen = dc.SelectObject(&pen);
	brush.CreateSolidBrush(0xcccccc); //ȸ�� ä��� �� ����
	pOldBrush = dc.SelectObject(&brush);
	dc.Rectangle(0, 40, client.right, client.bottom); // �簢�� �׸���
	dc.SelectObject(pOldBrush);

	pen.DeleteObject();
	brush.DeleteObject();

	//2. ������ �׸���.
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

	//2. CFont Ŭ���� ����
	CFont cFont;

	//3. ��Ʈ�� �����.
	cFont.CreateFont(
		this->textEditor->font->GetLogfont().lfHeight * this->zoomController->GetMagnification(),			//���ڳ���
		this->textEditor->font->GetLogfont().lfWidth,				//���ڳʺ�
		this->textEditor->font->GetLogfont().lfEscapement,		//��°���
		this->textEditor->font->GetLogfont().lfOrientation,		//���� �������� ����
		this->textEditor->font->GetLogfont().lfWeight,			//���ڱ���
		this->textEditor->font->GetLogfont().lfItalic,			//Italic ���뿩��
		this->textEditor->font->GetLogfont().lfUnderline,			//�������뿩��
		this->textEditor->font->GetLogfont().lfStrikeOut,			//��Ҽ����뿩��
		this->textEditor->font->GetLogfont().lfCharSet,			//���ڼ�����
		this->textEditor->font->GetLogfont().lfOutPrecision,		//������е�
		this->textEditor->font->GetLogfont().lfClipPrecision,		//Ŭ�������е�
		this->textEditor->font->GetLogfont().lfQuality,			//��¹���ǰ��
		this->textEditor->font->GetLogfont().lfPitchAndFamily,	//�۲�Pitch
		this->textEditor->font->GetLogfont().lfFaceName			//�۲�
	);

	//4. DC�� CFont Object ����
	dc.SelectObject(cFont);

	GlyphVisitor* previewTextOutVisitor = new PreviewTextOutVisitor(this, &dc, rectMargin.left, rectMargin.top);

	Glyph* note = this->notes.GetAt(currentPage - 1);
	note->Accept(previewTextOutVisitor);

	cFont.DeleteObject();

	if (previewTextOutVisitor != 0) {
		delete previewTextOutVisitor;
	}

	// ���� ������ �׸���.
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
	//4. DC�� CFont Object ����
	cFont.CreateFont(
		-12,			//���ڳ���
		0,				//���ڳʺ�
		0,				//��°���
		0,				//���� �������� ����
		700,			//���ڱ���
		'\0',			//Italic ���뿩��
		'\0',			//�������뿩��
		'\0',			//��Ҽ����뿩��
		'\x1',			//���ڼ�����
		'\0',			//������е�
		'\0',			//Ŭ�������е�
		'\0',			//��¹���ǰ��
		'\0',			//�۲�Pitch
		"�������"		//�۲�
	);
	dc.SelectObject(cFont);
	CString information;
	information.Format("(%d�� / %d��) ����: A4 [210 x 297 mm]", this->currentPage, this->GetLength());
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
	LONG paperWidth = this->paperX - this->textEditor->psd.rtMargin.left / 100 * (72 / 25.4F) - this->textEditor->psd.rtMargin.right / 100 * (72 / 25.4F); // ������ ������ ���� ��
	LONG paperHeight = this->paperY - this->textEditor->psd.rtMargin.top / 100 * (72 / 25.4F) - this->textEditor->psd.rtMargin.bottom / 100 * (72 / 25.4F); // ������ ������ ���� ����
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
		
		//1. textEditor�� ���̿��� �� �� ��ŭ �ݺ��ϴ�.
		i = 1;
		while (i <= this->textEditor->note->GetLength()) {
			//1.1. ���� ���ϴ�.
			row = this->textEditor->note->GetAt(i - 1);
			//1.2. preview���� ���� Y�� + ���ڳ��̰� ���̳��� ���� ũ��
			if (y + this->characterMetrics->GetHeight() > paperHeight) {
				//1.2.1. ���̸� ����� preview�� notes�� �߰��ϴ�.
				index = this->Add(glyphFactory.Create((char*)"\0"));
				y = 0;
				this->note = this->notes.GetAt(index);
			}
			//1.3. ���� �߰��ϴ�.
			if (!dynamic_cast<DummyRow*>(row)) {
				index = this->note->Add(glyphFactory.Create((char*)"\r\n"));
				y += this->characterMetrics->GetHeight();
				this->current = this->note->GetAt(index);
			}
			//1.4. ���� ���ڼ� ��ŭ �ݺ��ϴ�.
			j = 0;
			while (j < row->GetLength()) {
				//1.4.1. ���ڸ� ���ϴ�.
				character = row->GetAt(j);
				//1.4.2. ���� X�� ��ǥ + ���� ���� ������ ������ ũ��
				if (this->current->GetRowLength() + character->GetWidth(this->characterMetrics) > paperWidth) {
					//1.4.2.1. preview���� ���� Y�� + ���ڳ��̰� ���̳��� ���� ũ��
					if (y + this->characterMetrics->GetHeight() > paperHeight) {
						//1.4.2.1.1. ���̸� ����� preview�� notes�� �߰��ϴ�.
						index = this->Add(glyphFactory.Create((char*)"\0"));
						y = 0;
						this->note = this->notes.GetAt(index);
					}
					//1.4.2.2. ���̿��� ���� ����� �߰��ϴ�.
					index = this->note->Add(glyphFactory.Create((char*)"\r\n"));
					y += this->characterMetrics->GetHeight();
					this->current = this->note->GetAt(index);
				}
				//1.4.3. ������ ���ڸ� �߰��ϴ�.
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