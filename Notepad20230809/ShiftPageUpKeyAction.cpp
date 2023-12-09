//ShiftPageUpKeyAction.cpp
#include "ShiftPageUpKeyAction.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Scroll.h"
#include "Document.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ShiftPageUpKeyAction::ShiftPageUpKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ShiftPageUpKeyAction::~ShiftPageUpKeyAction() {

}

void ShiftPageUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	bool in = false;
	Long i = 0;
	Long count;
	Long height = this->textEditor->characterMetrics->GetHeight();
	Long vSMax;
	Long vSPage;
	Long vSPosition;
	Long vaildMin;
	Long vaildMax;
	Glyph* character;
	Position previous;
	Position current;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	vSMax = this->textEditor->scrollController->verticalScroll->GetMaximum();
	vSPage = this->textEditor->scrollController->verticalScroll->GetPage();
	vSPosition = this->textEditor->scrollController->verticalScroll->GetPosition();
	vaildMin = this->textEditor->scrollController->GetVaildMin();
	vaildMax = vaildMin + this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight() - vSPage;
	if (vaildMax < 0) {
		vaildMax = 0;
	}

	previous = this->textEditor->document->GetPosition();
	if (this->textEditor->isMoving == false) {
		this->textEditor->isMoving = true;
		this->textEditor->destination = this->textEditor->x;
	}
	this->textEditor->x = 0;
	if (vSPosition == 0) {
		this->textEditor->note->First();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->current->First();
	}
	else if (vSPosition - vSPage < 0) {
		vSPosition = this->textEditor->scrollController->verticalScroll->SetPosition(0);
		if (vSPosition < vaildMin) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			if (this->textEditor->isWrapped) {
				this->textEditor->memoryController->ScrollUp(true, vSPosition, vSPage, vaildMin, vaildMax);
			}
			else {
				this->textEditor->memoryController->ScrollUp(vSPosition, vSPage, vaildMin, vaildMax);
			}
		}
		this->textEditor->note->First();
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
			vSPosition = this->textEditor->scrollController->verticalScroll->SetPosition(vSPosition - count * height);
			i = this->textEditor->scrollController->below + this->textEditor->note->GetCurrent() - count;
		}
		else {
			i = (vSPosition - vSPage) / height + 1;
			vSPosition = this->textEditor->scrollController->verticalScroll->SetPosition((i - 1) * height);
		}
		if (vSPosition < vaildMin) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			if (this->textEditor->isWrapped) {
				this->textEditor->memoryController->ScrollUp(true, vSPosition, vSPage, vaildMin, vaildMax);
			}
			else {
				this->textEditor->memoryController->ScrollUp(vSPosition, vSPage, vaildMin, vaildMax);
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
	current = this->textEditor->document->GetPosition();
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}