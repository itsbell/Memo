//TextEditor.cpp
#include "TextEditor.h"

#include "Document.h"
#include "Glyph.h"
#include "Paper.h"
#include "Row.h"
#include "DummyRow.h"
#include "MultiByteCharacter.h"
#include "SingleByteCharacter.h"
#include "GlyphFactory.h"

#include "Font.h"
#include "CharacterMetrics.h"
#include "CaretController.h"
#include "ScrollController.h"
#include "MemoryController.h"
#include "ZoomController.h"
#include "StatusBarController.h"
#include "TextOutVisitor.h"
#include "SelectedTextOutVisitor.h"
#include "Stack.h"

#include "Command.h"
#include "TextEditorCommandFactory.h"
#include "KeyAction.h"
#include "KeyActionFactory.h"

#include "Scroll.h"
#include "HScrollAction.h"
#include "HScrollActionFactory.h"
#include "HorizontalScroll.h"
#include "VScrollAction.h"
#include "VScrollActionFactory.h"
#include "VerticalScroll.h"

#include "Position.h"
#include "FileMaker.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable:4996)
#include <afxext.h>

//ã��ٲٱ� ���̾�α׿� ���� ���޵Ǵ� �޼���
static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

BEGIN_MESSAGE_MAP(TextEditor, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CHAR()
	ON_MESSAGE(WM_IME_COMPOSITION, OnImeComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnImeStartComposition)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_COMMAND_RANGE(IDM_EDIT_UNDO, IDM_MAX, OnMenuClicked)
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplace)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSELEAVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

TextEditor::TextEditor()
	: pathName(""){
	this->document = 0;
	this->note = 0;
	this->current = 0;
	this->font = 0;
	this->characterMetrics = 0;
	this->caretController = 0;
	this->scrollController = 0;
	this->memoryController = 0;
	this->zoomController = 0;
	this->statusBarController = 0;
	this->unDoCommands = 0;
	this->reDoCommands = 0;
	this->findDialog = 0;
	this->findReplaceDialog = 0;
	this->rect = { 0, };
	this->isComposing = false;
	this->isModified = false;
	this->isMoving = false;
	this->isWrapped = false;
	this->isClicking = false;
	this->isUpdated = false;
	this->isScrolling = false;
	this->row = 1;
	this->column = 0;
	this->wrappedRow = 1;
	this->wrappedColumn = 0;
	this->scrollFlags = SF_ALL;
	this->matchCase = FALSE;
	this->matchWholeWord = FALSE;
	this->longestRow = 0;
	this->destination = 0;
	this->x = 0;
	this->y = 0;
	this->fileName = "���� ����";
	this->preview = 0;
}

TextEditor::~TextEditor() {
	
	if (this->document != 0) {
		delete this->document;
	}
	if (this->note != 0) {
		delete this->note;
	}
	if (this->font != 0) {
		delete this->font;
	}
	if (this->characterMetrics != 0) {
		delete this->characterMetrics;
	}
	if (this->scrollController != 0) {
		delete this->scrollController;
	}
	if (this->memoryController != 0) {
		delete this->memoryController;
	}
	if (this->statusBarController != 0) {
		delete this->statusBarController;
	}
	if (this->zoomController != 0) {
		delete this->zoomController;
	}
	if (this->unDoCommands != 0) {
		delete this->unDoCommands;
	}
	if (this->reDoCommands != 0) {
		delete this->reDoCommands;
	}
	//preview�� preview �����찡 ����Ǹ鼭 �Ҵ������˴ϴ�.
	//caretController�� KillFocus �޼������� �Ҵ������մϴ�.
}

/*
�Լ��̸�: GetX
��    ��: ���� ��ġ ��ǥ(X)�� ���ϴ�.
��    ��: ��, ĭ
��    ��: ��ǥ(X)
*/
Long TextEditor::GetX(Long row, Long column) {
	Long i = 0;
	Long x = 0;
	Glyph* current = this->note->GetAt(row - 1);
	Glyph* character;

	//1. ó������ column���� �ݺ��ϴ�.
	while (i < column) {
		
		//1.1. ������ġ ���ڸ� ���ϴ�.
		character = current->GetAt(i);
		x += character->GetWidth(this->characterMetrics);

		i++;
	}

	return x;
}

/*
�Լ��̸�: GetY
��    ��: ���� ��ġ ��ǥ(Y)�� ���ϴ�.
��    ��: ��
��    ��: ��ǥ(Y)
*/
Long TextEditor::GetY(Long row) {
	Long y = 0;

	y = (row - 1) * this->characterMetrics->GetHeight();

	return y;
}

Long TextEditor::GetY() {
	Long y;
	
	if (this->document->GetLength() > 0) {
		y = (this->scrollController->GetBelow() + this->note->GetCurrent() - 1) * this->characterMetrics->GetHeight();
	}

	else {
		y = (this->note->GetCurrent() - 1) * this->characterMetrics->GetHeight();
	}

	return y;
}

void TextEditor::SetCurrent() {
	Long i = this->note->GetCurrent();
	Glyph* glyph;

	this->wrappedRow = 0;
	this->wrappedColumn = 0;
	if (this->isWrapped) {
		this->wrappedRow = this->note->GetCurrent() + this->scrollController->below;
		this->wrappedColumn = this->current->GetCurrent();
	}
	glyph = this->note->GetAt(i - 1);
	this->column = glyph->GetCurrent();
	while (dynamic_cast<DummyRow*>(glyph)) {
		i--;
		this->column += this->note->GetAt(i - 1)->GetLength();
		glyph = this->note->GetAt(i - 1);
	}
	this->row = this->document->GetRowNumber();
}

void TextEditor::GetBack() {
	Long i;
	Glyph* glyph;
	
	if (this->row < this->document->GetStart() || this->row > this->document->GetEnd()) {
		if (this->row < this->document->GetStart()) {
			this->memoryController->MoveUp(this->row);
			this->note->First();
		}
		else {
			this->memoryController->MoveDown(this->row - this->note->GetRowCount() + 1);
			this->note->Move(this->note->GetCloseRow(this->note->GetCurrent()));
		}
	}
	else {
		i = this->document->GetRowNumber(true, this->row);
		this->note->Move(i);
	}
	glyph = this->note->GetAt(this->note->GetCurrent() - 1);
	while (this->column > glyph->GetLength()) {
		this->column -= glyph->GetLength();
		this->note->Next();
		glyph = this->note->GetAt(this->note->GetCurrent() - 1);
	}
	glyph->Move(this->column);
	this->current = glyph;
	this->isScrolling = false;
}

int TextEditor::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);
	
	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_ONCREATE, NULL));
	
	return 0;
}

void TextEditor::OnPaint() {
	Long i;
	Long j;
	Long y = 0;
	CPaintDC dc(this);
	CDC memoryDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;
	CFont cFont;
	GlyphVisitor* textOutVisitor = 0;
	GlyphVisitor* selectedTextOutVisitor = 0;
	Long vSPosition = this->scrollController->verticalScroll->GetPosition();
	Long vSPage = this->scrollController->verticalScroll->GetPage();

	memoryDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBitmap = memoryDC.SelectObject(&bitmap);
	memoryDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);

	//3. ��Ʈ�� �����.

	cFont.CreateFont(
		font->GetLogfont().lfHeight * this->zoomController->GetMagnification(),			//���ڳ���
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

	//4. DC�� CFont Object ����
	memoryDC.SelectObject(cFont);

	if (this->scrollController->verticalScroll->GetPosition() % this->characterMetrics->GetHeight() > 0) {
		y = -(this->scrollController->verticalScroll->GetPosition() % this->characterMetrics->GetHeight());
	}
	// ȭ�鿡 ��� ������ ���� ���ϴ�.
	i = vSPosition / this->characterMetrics->GetHeight();
	
	// ȭ�鿡 ��µ� ���������� ���ϴ�.
	j = (vSPosition + vSPage) / this->characterMetrics->GetHeight();
	if ((vSPosition + vSPage) % this->characterMetrics->GetHeight() > 0) {
		j++;
	}
	i -= this->scrollController->GetBelow();
	j -= this->scrollController->GetBelow();
#ifdef _DEBUG
	if (i < 0) {
		TRACE("problem TextEditor::OnPaint\n");
	}
#endif
	//��½� ����� �����ϰ� �Ͽ� ���ڰ�ħ�� ���ִ�.
	memoryDC.SetBkMode(TRANSPARENT);
	textOutVisitor = new TextOutVisitor(this, &memoryDC, y, i, j);
	this->note->Accept(textOutVisitor);
	//��� �� ����� �ٽ� ������� ��ġ��..
	memoryDC.SetBkMode(OPAQUE);
	memoryDC.SetBkColor(0x00ebc697);
	if (this->document->isSelecting) {
		selectedTextOutVisitor = new SelectedTextOutVisitor(this, &memoryDC, y, i, j);
		this->note->Accept(selectedTextOutVisitor);
	}
	
	dc.BitBlt(0, 0, this->rect.Width(), this->rect.Height(), &memoryDC, 0, 0, SRCCOPY);
	dc.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	ReleaseDC(&memoryDC);
	DeleteDC(memoryDC);

	//7. CFont Object ����
	cFont.DeleteObject();
	if (textOutVisitor != 0) {
		delete textOutVisitor;
	}
	if (selectedTextOutVisitor != 0) {
		delete selectedTextOutVisitor;
	}
}

/*
�Լ��̸�: OnChar
��    ��: Ű���带 ���� �Էµ� ���� ���� ó���ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void TextEditor::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	
	if (nChar == 9 || nChar == 13 || (nChar >= 32 && nChar <= 126)) {
		this->character[0] = nChar;
		this->character[1] = '\0';
		bool isSelecting = this->document->isSelecting;

		if ((nChar == 9 || nChar >= 32 && nChar <= 126) && isSelecting == false) {
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_CHAR, NULL));
		}
		else if(nChar == 13 && isSelecting == false) {
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_CHARENTER, NULL));
		}
		else if ((nChar == 9 || nChar >= 32 && nChar <= 126) && isSelecting) {
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_CHARSELECTING, NULL));
		}
		else if (nChar == 13 && isSelecting) {
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_CHARENTERSELECTING, NULL));
		}
	}
}

/*
�Լ��̸�: OnImeComposition
��    ��: Ű���带 ���� �Էµ� ���� ���� ó���ϴ�.
��    ��: wParam, lParam
��    ��: ::DefWindowProc(this->m_hWnd,WM_IME_COMPOSITION,wParam,lParam)
*/
LRESULT TextEditor::OnImeComposition(WPARAM wParam, LPARAM lParam) {
	this->character[0] = wParam / 256;
	this->character[1] = wParam % 256;

	if (this->document->isSelecting == false) {
		this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_IMECOMPOSITION, NULL));
	}
	else {
		this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_IMECOMPOSITIONSELECTING, NULL));
	}

	return ::DefWindowProc(this->m_hWnd,WM_IME_COMPOSITION,wParam,lParam);
}

/*
�Լ��̸�: OnImeChar
��    ��: ���ڰ� �ϼ��Ǹ� ������ ���¿� ĳ���� ��ġ��.
��    ��: wParam, lParam
��    ��: LRESULT
*/
LRESULT TextEditor::OnImeChar(WPARAM wParam, LPARAM lParam) {

	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_IMECHAR, NULL));

	return 0;
}

/*
�Լ��̸�: OnImeStartComposition
��    ��: ���� �Է��� ó�� ������ �� ������ ���¸� ��ġ��.
��    ��: wParam, lParam
��    ��: LRESULT
*/
LRESULT TextEditor::OnImeStartComposition(WPARAM wParam, LPARAM lParam) {

	//1. �������� �ƴ� ���·� �ٲٴ�.
	this->isComposing = false;

	return 0;
}


void TextEditor::OnSetFocus(CWnd* pOldWnd) {

	if (this->caretController == 0) {
		this->caretController = new CaretController(this);
	}
	this->Notify();
}

void TextEditor::OnKillFocus(CWnd* pNewWnd) {

	if (this->caretController != 0) {
		delete this->caretController;
		this->caretController = 0;
	}
}

BOOL TextEditor::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	
	//Ŭ���̾�Ʈ ���� ���̸�
	if (nHitTest == HTCLIENT) {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
		return TRUE;
	}
	
	return CWnd::OnSetCursor(pWnd,nHitTest,message);
}

void TextEditor::OnLButtonDown(UINT nFlags, CPoint point) {
	this->nFlags = nFlags;
	this->point = point;
	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_LBUTTONDOWN, NULL));
}

void TextEditor::OnLButtonUp(UINT nFlags, CPoint point) {
	this->nFlags = nFlags;
	this->point = point;
	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_LBUTTONUP, NULL));
}

void TextEditor::OnLButtonDblClk(UINT nFlags, CPoint point) {
	this->nFlags = nFlags;
	this->point = point;
	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_LBUTTONDBLCLK, NULL));
}

void TextEditor::OnMouseMove(UINT nFlags, CPoint point) {
	if (::GetCapture() == this->m_hWnd && this->isClicking) {
		this->nFlags = nFlags;
		this->point = point;
		this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_MOUSEMOVE, NULL));
	}
}


void TextEditor::OnRButtonUp(UINT nFlags, CPoint point)
{
	this->nFlags = nFlags;
	this->point = point;
	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_RBUTTONUP, NULL));
}

#if 0
void TextEditor::OnMouseLeave() {
	this->isClicking = false;
}
#endif

BOOL TextEditor::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	this->nFlags = nFlags;
	this->zDelta = zDelta;
	this->point = pt;
	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_MOUSEWHEEL, NULL));
	
	return TRUE;
}

void TextEditor::OnMenuClicked(UINT nID) {
	Command* command;
	TextEditorCommandFactory commandFactory(this);

	command = commandFactory.Create(nID);

	if (command != 0) {
		//�α� ����� �ڵ� �Դϴ�.
		FileMaker fileMaker(this);
		fileMaker.RecordLog(nID);

		command->Execute();
		delete command;
	}
}

void TextEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	KeyAction* keyAction;
	KeyActionFactory keyActionFactory(this);

	keyAction = keyActionFactory.Create(nChar);
	
	if (keyAction != 0) {
		//�α� ����� �ڵ� �Դϴ�.
		FileMaker fileMaker(this);
		fileMaker.RecordLog(nChar, true);

		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		delete keyAction;
	}
}

void TextEditor::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	HScrollAction* hScrollAction;
	HScrollActionFactory hScrollActionFactory(this);

	hScrollAction = hScrollActionFactory.Create(nSBCode);
	if (hScrollAction != 0) {
		hScrollAction->OnHScroll(nSBCode, nPos, pScrollBar);
		delete hScrollAction;
	}
}

void TextEditor::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	VScrollAction* vScrollAction;
	VScrollActionFactory vScrollActionFactory(this);

	vScrollAction = vScrollActionFactory.Create(nSBCode);
	if (vScrollAction != 0) {
		vScrollAction->OnVScroll(nSBCode, nPos, pScrollBar);
		delete vScrollAction;
	}
}

void TextEditor::OnSize(UINT nType, int cx, int cy) {
	Long i;
	Long j;
	Long position;
	Scroll* hS = this->scrollController->horizontalScroll;
	Scroll* vS = this->scrollController->verticalScroll;
	GlyphFactory glyphFactory;

	this->GetClientRect(&this->rect);
	if (this->isWrapped) {
		if (this->isScrolling == false) {
			i = this->document->GetRowNumber(false, this->note->GetCurrent());
			j = this->note->GetColumnNumber();
		}
		else {
			i = this->row;
			j = this->column;
		}
		this->note->UnWrap(this->characterMetrics);
		this->note->Wrap(this->rect.right, this->characterMetrics);
		this->scrollController->UpdateFileVSInfo(true);
		if (this->isScrolling == false || (this->isScrolling == true && i >= this->document->GetStart() && i <= this->document->GetEnd())) {
			this->document->Move(i, j);
			this->current = this->note->GetAt(this->note->GetCurrent() - 1);
			if (this->isComposing == false) {
				this->x = this->GetX(this->note->GetCurrent(), this->current->GetCurrent());
			}
			else if (this->current->GetCurrent() > 0 && strcmp(this->character, "") != 0) {
				this->x = this->GetX(this->note->GetCurrent(), this->current->GetCurrent() - 1);
			}
			this->y = this->GetY();
		}
		this->scrollFlags = SF_WRAP;
		this->scrollController->UpdateMaximum();
		if (hS->GetMaximum() < this->rect.right) {
			hS->SetPosition(0);
		}
		else if (hS->GetPosition() > hS->GetMaximum() - this->rect.right) {
			hS->SetPosition(hS->GetMaximum() - this->rect.right);
		}
		if (vS->GetPosition() < this->scrollController->GetVaildMin()) {
			vS->SetPosition(this->scrollController->GetVaildMin());
		}
		else if (vS->GetPosition() > vS->GetMaximum() - this->rect.bottom) {
			position = vS->GetMaximum() - this->rect.bottom;
			if (position < 0) {
				position = 0;
			}
			vS->SetPosition(position);
		}
	}
	else {
		this->scrollController->UpdateMaximum();
		if (hS->GetMaximum() < this->rect.right) {
			hS->SetPosition(0);
		}
		else if (hS->GetPosition() > hS->GetMaximum() - this->rect.right) {
			hS->SetPosition(hS->GetMaximum() - this->rect.right);
		}
		if (vS->GetPosition() < this->scrollController->GetVaildMin()) {
			vS->SetPosition(this->scrollController->GetVaildMin());
		}
		else if (vS->GetPosition() > vS->GetMaximum() - this->rect.bottom) {
			position = vS->GetMaximum() - this->rect.bottom;
			if (position < 0) {
				position = 0;
			}
			vS->SetPosition(position);
		}
	}
	this->Notify();
	this->Invalidate(FALSE);
}

LRESULT TextEditor::OnFindReplace(WPARAM wParam, LPARAM lParam) {
	this->wParam = wParam;
	this->lParam = lParam;
	UNREFERENCED_PARAMETER(wParam);
	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FINDREPLACE, NULL));

#if 0
	CFindReplaceDialog* pDlg = pDlg->GetNotifier(lParam);

	if (pDlg != NULL) {
		
		if (pDlg->FindNext()) {
			this->findWhat = pDlg->m_fr.lpstrFindWhat;
			this->replaceWith = pDlg->m_fr.lpstrReplaceWith;
			this->matchCase = pDlg->MatchCase();
			this->matchWholeWord = pDlg->MatchWholeWord();

			if (pDlg->SearchDown()) {
				this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_FINDNEXT, NULL));
			}

			else {
				this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_FINDPREVIOUS, NULL));
			}
		}

		else if (pDlg->ReplaceCurrent()) {
			this->findWhat = pDlg->m_fr.lpstrFindWhat;
			this->replaceWith = pDlg->m_fr.lpstrReplaceWith;
			this->matchCase = pDlg->MatchCase();
			this->matchWholeWord = pDlg->MatchWholeWord();

			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_REPLACEFIND, NULL));
		}

		else if (pDlg->ReplaceAll()) {
			this->findWhat = pDlg->m_fr.lpstrFindWhat;
			this->replaceWith = pDlg->m_fr.lpstrReplaceWith;
			this->matchCase = pDlg->MatchCase();
			this->matchWholeWord = pDlg->MatchWholeWord();

			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_REPLACEALL, NULL));
		}

		pDlg->SetFocus();

		if (pDlg->IsTerminating()) {
		
			if (this->findDialog != 0) {
				this->findDialog = 0;
			}

			if (this->findReplaceDialog != 0) {
				this->findReplaceDialog = 0;
			}	
		}
	}
#endif
	return 0;
}


void TextEditor::OnDestroy() {
	FileMaker fm(this);

	fm.RecordLog("OnDestroy");
	fm.Remove();
}