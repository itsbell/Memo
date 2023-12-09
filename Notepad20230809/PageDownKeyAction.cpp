//PageDownKeyAction.cpp
#include "PageDownKeyAction.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "HorizontalScroll.h"
#include "VerticalScroll.h"
#include "Glyph.h"
#include "Document.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PageDownKeyAction::PageDownKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

PageDownKeyAction::~PageDownKeyAction() {

}

void PageDownKeyAction::OnKeyDown(UINT nChar, UINT nRecpCnt, UINT nFlags) {
	bool in = false;
	Long i = 0;
	Long count;
	Long height = this->textEditor->characterMetrics->GetHeight();
	Long vSMax = this->textEditor->scrollController->verticalScroll->GetMaximum();
	Long vSPage = this->textEditor->scrollController->verticalScroll->GetPage();
	Long vSPosition = this->textEditor->scrollController->verticalScroll->GetPosition();
	Long vaildMin = this->textEditor->scrollController->GetVaildMin();
	Long vaildMax = vaildMin + this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight() - vSPage;
	Glyph* character;

	if (vaildMax < 0) {
		vaildMax = 0;
	}
	if (this->textEditor->document->isSelecting) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}

	if (this->textEditor->isMoving == false) {
		this->textEditor->isMoving = true;
		this->textEditor->destination = this->textEditor->x;
	}
	this->textEditor->x = 0;
	if (vSPosition == vSMax - vSPage) {
		this->textEditor->note->Last();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->current->Last();
	}
	else if (vSPosition + vSPage > vSMax - vSPage) {
		vSPosition = this->textEditor->scrollController->verticalScroll->SetPosition(vSMax - vSPage);
		if (vSPosition > vaildMax) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			if (this->textEditor->isWrapped) {
				this->textEditor->memoryController->ScrollDown(true, vSPosition, vSPage, vaildMin, vaildMax);
			}
			else {
				this->textEditor->memoryController->ScrollDown(vSPosition, vSPage, vaildMin, vaildMax);
			}
		}
		this->textEditor->note->Last();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
			character = this->textEditor->current->GetAt(i);
			this->textEditor->x += character->GetWidth(this->textEditor->characterMetrics);
			i++;
		}
		this->textEditor->current->Move(i);
	}
	else {
		if (this->textEditor->y >= vSPosition && this->textEditor->y <= vSPosition + vSPage - height) {
			in = true;
		}
		if (in) {
			count = this->textEditor->rect.bottom / height;
			vSPosition = this->textEditor->scrollController->verticalScroll->SetPosition(vSPosition + count * height);
			i = this->textEditor->scrollController->below + this->textEditor->note->GetCurrent() + count;
		}
		else {
			i = (vSPosition + vSPage) / height + 1;
			vSPosition = this->textEditor->scrollController->verticalScroll->SetPosition((i - 1) * height);
		}
		if (vSPosition > vaildMax) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			if (this->textEditor->isWrapped) {
				this->textEditor->memoryController->ScrollDown(true, vSPosition, vSPage, vaildMin, vaildMax);
			}
			else {
				this->textEditor->memoryController->ScrollDown(vSPosition, vSPage, vaildMin, vaildMax);
			}
		}
		if (in) {	
			i -= this->textEditor->scrollController->below;
			this->textEditor->note->Move(i);
			this->textEditor->current = this->textEditor->note->GetAt(i - 1);
			i = 0;
			while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
				character = this->textEditor->current->GetAt(i);
				this->textEditor->x += character->GetWidth(this->textEditor->characterMetrics);
				i++;
			}
			this->textEditor->current->Move(i);
		}
		else {
			i -= this->textEditor->scrollController->below;
			this->textEditor->note->Move(i);
			this->textEditor->current = this->textEditor->note->GetAt(i - 1);
			this->textEditor->current->First();
		}
	}
	this->textEditor->isScrolling = false;
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);

}