//FontCommand.cpp
#include "FontCommand.h"


#include <string>
using namespace std;
#include "TextEditor.h"
#include "Font.h"
#include "CharacterMetrics.h"
#include "Glyph.h"
#include "SingleByteCharacter.h"
#include "MultiByteCharacter.h"
#include "Document.h"
#include "ScrollController.h"
#include "resource.h"
#include "MemoryController.h"
#include "FileMaker.h"
#include "Scroll.h"
#include <afxdlgs.h>
#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FontCommand::FontCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

FontCommand::~FontCommand() {

}

void FontCommand::Execute() {
	int ret;
	char buffer[MAX];
	Long i;
	Long end;
	Long max;
	FILE* file;
	Glyph* glyph;
	string character;
	FileMaker fm(this->textEditor);
	LOGFONT lf = this->textEditor->font->GetLogfont();
	CFontDialog fontDialog(&lf, CF_SCREENFONTS, NULL, NULL);
	
	//1. 폰트 대화상자를 띄우다.
	ret = fontDialog.DoModal();

	//2. 확인을 눌르면
	if (ret == IDOK) {

		//2.1. 글꼴 정보를 읽는다.
		fontDialog.GetCurrentFont(&lf);
		if (lf.lfHeight < -165) {
			lf.lfHeight = -165;
		}
		else if (lf.lfHeight > -2) {
			lf.lfHeight = -2;
		}
		//2.2. textEditor의 font와 characterMetrics를 다시 생성하다.
		if (this->textEditor->font != 0 && this->textEditor->characterMetrics != 0) {
			delete this->textEditor->font;
			delete this->textEditor->characterMetrics;
			this->textEditor->font = new Font(lf);
			this->textEditor->characterMetrics = new CharacterMetrics(this->textEditor->GetDC(), this->textEditor->font, this->textEditor->zoomController);
		}
		if (this->textEditor->isScrolling) {
			this->textEditor->GetBack();
		}
		if (this->textEditor->isWrapped) {
			this->textEditor->note->UnWrap(this->textEditor->characterMetrics);
			this->textEditor->note->Wrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
		if (this->textEditor->isUpdated) {
			this->textEditor->memoryController->SaveRowLength(true);
		}
		else {
			this->textEditor->memoryController->SaveRowLength();
			end = this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1;
			this->textEditor->document->SetEnd(end);
		}
		this->textEditor->scrollController->UpdateFileVSInfo(true);
		
		if (this->textEditor->isWrapped) {
			this->textEditor->scrollFlags = SF_WRAP;
		}
		else {
			if (this->textEditor->longestRow < this->textEditor->document->GetStart() || this->textEditor->longestRow > this->textEditor->document->GetEnd()) {
				file = fopen(fm.GetFile(), "rt");
				if (file != NULL) {
					fseek(file, (this->textEditor->longestRow - 1) * (12 + MAX) + 5, SEEK_SET);
					fscanf(file, "%d", &max);
					fclose(file);
				}
			}
			else {
				glyph = this->textEditor->note->GetAt(this->textEditor->document->GetRowNumber(true, this->textEditor->longestRow) - 1);
				max = glyph->GetRowLength();
			}
			this->textEditor->scrollController->horizontalScroll->SetMaximum(max);
			this->textEditor->scrollFlags = SF_NORMAL;
		}
		this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
		this->textEditor->y = this->textEditor->GetY();
		this->textEditor->scrollController->UpdateMaximum();
		this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
		this->textEditor->Notify();
		this->textEditor->Invalidate(FALSE);
	}
}

void FontCommand::Unexecute() {

}