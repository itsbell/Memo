//ShiftHomeKeyAction.cpp
#include "ShiftHomeKeyAction.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Position.h"
#include "Document.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ShiftHomeKeyAction::ShiftHomeKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ShiftHomeKeyAction::~ShiftHomeKeyAction() {

}

void ShiftHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Position previous;
	Position current;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	previous = this->textEditor->document->GetPosition();
	this->textEditor->isMoving = false;
	this->textEditor->current->First();
	this->textEditor->x = 0;
	current = this->textEditor->document->GetPosition();
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}