//CtrlDKeyAction.cpp
#include "CtrlDKeyAction.h"
#include "TextEditor.h"
#include "Document.h"
#include "Scroll.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Font.h"
#pragma warning (disable : 4996)

CtrlDKeyAction::CtrlDKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlDKeyAction::~CtrlDKeyAction() {

}

void CtrlDKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
#ifdef _DEBUG
	char isUpdated[6];
	char isScrolling[6];
	char isSelecting[6];
	CString info;
	Document* document = this->textEditor->document;
	Scroll* HS = this->textEditor->scrollController->horizontalScroll;
	Scroll* VS = this->textEditor->scrollController->verticalScroll;

	if (this->textEditor->isUpdated) {
		strcpy(isUpdated, "true");
	}
	else {
		strcpy(isUpdated, "false");
	}
	if (this->textEditor->isScrolling) {
		strcpy(isScrolling, "true");
	}
	else {
		strcpy(isScrolling, "false");
	}
	if (document->isSelecting) {
		strcpy(isSelecting, "true");
	}
	else {
		strcpy(isSelecting, "false");
	}

	info.Format("#TextEditor\n- isUpdaed : %s\n- isScrolling : %s\n\n#Document\n- start : %d\n- end : %d\n- length : %d\n- isSelecting : %s\n- startPosition : (%d,%d)\n- endPosition : (%d,%d)\n\n#ScrollController\n- below : %d\n- VSMax : %d\n- diskFileVSMax : %d\n- vaildMin : %d\n\n#Paper\n- length : %d\n- rowCount : %d\n- current : %d\n\n#HS (LongestRow : %d)\n- maximum : %d\n- page : %d\n- position : %d\n\n#VS\n- maximum : % d\n- page : % d\n- position : %d",
		isUpdated, isScrolling, this->textEditor->document->GetStart(), this->textEditor->document->GetEnd(), this->textEditor->document->GetLength(), isSelecting, document->startPosition.row, document->startPosition.column, document->endPosition.row, document->endPosition.column, this->textEditor->scrollController->below, this->textEditor->scrollController->vSMax, this->textEditor->scrollController->diskFileVSMax, this->textEditor->scrollController->GetVaildMin(), this->textEditor->note->GetLength(), this->textEditor->note->GetRowCount(), this->textEditor->note->GetCurrent(),this->textEditor->longestRow, HS->GetMaximum(), HS->GetPage(), HS->GetPosition(),
		VS->GetMaximum(), VS->GetPage(), VS->GetPosition());
	
	this->textEditor->MessageBox(info, "Infomation");

	CString font;
	LOGFONT lf = this->textEditor->font->GetLogfont();
	font.Format("lfHeight: %05d\nlfWidth: %05d\nlfEscapement: %05d\nlfOrientation: %05d\nlfWeight: %05d\nlfItalic: %05d\nlfUnderline: %05d\n"
		"lfStrikeOut: %05d\nlfCharSet: %05d\nlfOutPrecision: %05d\nlfClipPrecision: %05d\nlfQuality: %05d\nlfPitchAndFamily: %05d\nlfFaceName: %s",
		lf.lfHeight, lf.lfWeight, lf.lfEscapement, lf.lfOrientation, lf.lfWeight, lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet, lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality, lf.lfPitchAndFamily, lf.lfFaceName);
	this->textEditor->MessageBox(font, "Font");

#endif
}