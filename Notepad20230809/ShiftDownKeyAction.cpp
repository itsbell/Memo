//ShiftDownKeyAction.cpp
#include "ShiftDownKeyAction.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "MemoryController.h"
#include "Document.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ShiftDownKeyAction::ShiftDownKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ShiftDownKeyAction::~ShiftDownKeyAction() {

}

void ShiftDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long i = 0;
	Position previous;
	Position current;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	previous = this->textEditor->document->GetPosition();
	if (this->textEditor->isMoving == false) {
		this->textEditor->isMoving = true;
		this->textEditor->destination = this->textEditor->x;
	}
	if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength()) {
		this->textEditor->note->Next();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->y += this->textEditor->characterMetrics->GetHeight();
		this->textEditor->x = 0;
		while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
			this->textEditor->x += this->textEditor->current->GetAt(i)->GetWidth(this->textEditor->characterMetrics);
			i++;
		}
		this->textEditor->current->Move(i);
	}

	else {
		if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength()) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			this->textEditor->memoryController->MoveDown(this->textEditor->document->GetStart() + 1);
			this->textEditor->note->Move(this->textEditor->note->GetCloseRow(this->textEditor->note->GetCurrent()));
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->y += this->textEditor->characterMetrics->GetHeight();
			this->textEditor->x = 0;
			while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
				this->textEditor->x += this->textEditor->current->GetAt(i)->GetWidth(this->textEditor->characterMetrics);
				i++;
			}
			this->textEditor->current->Move(i);
		}
		else {
			this->textEditor->current->Last();
			this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
			this->textEditor->destination = this->textEditor->x;
		}
	}
	current = this->textEditor->document->GetPosition();
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}