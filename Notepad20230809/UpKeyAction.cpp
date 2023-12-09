//UpKeyAction.cpp
#include "UpKeyAction.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "Document.h"
#include "MemoryController.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UpKeyAction::UpKeyAction() {

}

UpKeyAction::UpKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

UpKeyAction::~UpKeyAction() {

}

/*
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void UpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long i;
	Long row;

	if (this->textEditor->isScrolling == true) {
		this->textEditor->GetBack();
		this->textEditor->isScrolling = false;
	}
	//2. ������ �����̸� ���������ϴ�.
	if (this->textEditor->document->isSelecting == true) {
		this->textEditor->note->UnSelect();
		this->textEditor->document->isSelecting = false;
	}
	//1. �̵����� �ƴϸ�
	if (this->textEditor->isMoving == false) {
		this->textEditor->destination = this->textEditor->x;
		this->textEditor->isMoving = true;
	}

	//3. ���� ���� ������ ù��° ���� �ƴϸ鼭 ������ ù��° ���̸�
	if (this->textEditor->document->GetStart() > 1 && this->textEditor->note->GetCurrent() == 1) {	
		//3.1. �޸� ������Ʈ�� �ȵ� �����̸�
		if (this->textEditor->isUpdated == false) {
			this->textEditor->memoryController->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		//3.2. �޸���Ʈ�ѷ����� ���η� �̵��ϴ�.
		this->textEditor->memoryController->MoveUp(this->textEditor->document->GetStart() - 1);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		//3.3. X��ǥ�� ���ϴ�.
		this->textEditor->x = 0;
		i = 0;
		while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
			this->textEditor->x += this->textEditor->current->GetAt(i)->GetWidth(this->textEditor->characterMetrics);
			i++;
		}
		//3.4. �ٿ��� �̵��ϴ�.
		this->textEditor->current->Move(i);
	}

	//4. �������� ù ��° ���� �ƴϸ�
	else if (this->textEditor->note->GetCurrent() > 1) {
		//4.1. ���� ���� ���� �ٷ� �̵��ϴ�.
		row = this->textEditor->note->Previous();
		this->textEditor->current = this->textEditor->note->GetAt(row - 1);
		//4.2. ��ǥ(X)�� ���ϴ�.
		this->textEditor->x = 0;
		i = 0;
		while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
			this->textEditor->x += this->textEditor->current->GetAt(i)->GetWidth(this->textEditor->characterMetrics);
			i++;
		}
		//4.3. ���� �ٿ��� ĭ�� �̵��ϴ�.
		this->textEditor->current->Move(i);
	}
	
	this->textEditor->y = this->textEditor->GetY();
	//5. ������Ʈ�ϴ�. (��ũ��, ĳ��)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();

	//6. ȭ���� ������Ʈ�ϴ�.
	this->textEditor->Invalidate(FALSE);
}