//CtrlShiftHomeKeyAction.cpp
#include "CtrlShiftHomeKeyAction.h"
#include "TextEditor.h"
#include "MemoryController.h"
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

CtrlShiftHomeKeyAction::CtrlShiftHomeKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CtrlShiftHomeKeyAction::~CtrlShiftHomeKeyAction() {

}

void CtrlShiftHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Position previous;
	Position current;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	previous = this->textEditor->document->GetPosition();
	this->textEditor->isMoving = false;
	if (this->textEditor->document->GetStart() > 1) {
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->textEditor->memoryController->MoveUp(1);
	}
	this->textEditor->note->First();
	this->textEditor->current = this->textEditor->note->GetAt(0);
	this->textEditor->current->First();
	this->textEditor->x = 0;
	this->textEditor->y = 0;
	
	current = this->textEditor->document->GetPosition();
	//1.5. ���̿��� �����ϴ�.
	this->textEditor->document->Select(previous, current);
	this->textEditor->document->UpdateSelectingInfo(previous, current);

	//1.6. ��ũ���������� ������Ʈ�ϴ�.
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);

	//1.7. ������Ʈ�ϴ�.
	this->textEditor->Notify();

	//1.8. ȭ���� �����ϴ�.
	this->textEditor->Invalidate(FALSE);
	
}