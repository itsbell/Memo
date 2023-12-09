//ShiftUpKeyAction.cpp
#include "ShiftUpKeyAction.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "CharacterMetrics.h"
#include "Glyph.h"
#include "Document.h"
#include "Position.h"
#include "MemoryController.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ShiftUpKeyAction::ShiftUpKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ShiftUpKeyAction::~ShiftUpKeyAction() {

}

void ShiftUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
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
	if (this->textEditor->note->GetCurrent() > 1){
		this->textEditor->note->Previous();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->y -= this->textEditor->characterMetrics->GetHeight();
		this->textEditor->x = 0;
		while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
			this->textEditor->x += this->textEditor->current->GetAt(i)->GetWidth(this->textEditor->characterMetrics);
			i++;
		}
		this->textEditor->current->Move(i);
	}
	else {
		if (this->textEditor->document->GetStart() > 1) {
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->y -= this->textEditor->characterMetrics->GetHeight();
			this->textEditor->x = 0;
			while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
				this->textEditor->x += this->textEditor->current->GetAt(i)->GetWidth(this->textEditor->characterMetrics);
				i++;
			}
			this->textEditor->current->Move(i);
		}
		else {
			this->textEditor->current->First();
			this->textEditor->x = 0;
			this->textEditor->destination = 0;
		}
	}
	current = this->textEditor->document->GetPosition();
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}