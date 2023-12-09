//ShiftRightKeyAction.cpp
#include "ShiftRightKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "DummyRow.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Document.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ShiftRightKeyAction::ShiftRightKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

ShiftRightKeyAction::~ShiftRightKeyAction() {

}

void ShiftRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Position previous;
	Position current;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	previous = this->textEditor->document->GetPosition();
	this->textEditor->isMoving = false;
	if (this->textEditor->current->GetCurrent() < this->textEditor->current->GetLength()) {
		this->textEditor->current->Next();
		if (this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength() &&
			this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength() &&
			dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(this->textEditor->note->GetCurrent()))) {

			this->textEditor->note->Next();
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->current->First();
		}
	}

	//3. �������� ���������� �ƴϰ� ����ĭ�� ������ĭ�̸�
	else if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength() &&
		this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {

		//3.1. �����ٷ� �̵��ϴ�.
		this->textEditor->note->Next();
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//3.2. �ٿ��� ó������ �̵��ϴ�.
		this->textEditor->current->First();
	}
	else if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength() &&
		this->textEditor->note->GetCurrent() == this->textEditor->note->GetLength() &&
		this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {

		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveDown(this->textEditor->document->GetStart() + 1);
		this->textEditor->note->Move(this->textEditor->note->GetCloseRow(this->textEditor->note->GetCurrent()));
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->current->First();
	}
	current = this->textEditor->document->GetPosition();
	//5. ���̿��� �����ϴ�.
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}