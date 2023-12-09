//DownKeyAction.cpp
#include "DownKeyAction.h"
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

DownKeyAction::DownKeyAction() {

}

DownKeyAction::DownKeyAction(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

DownKeyAction::~DownKeyAction() {

}

/*
�Լ��̸�: OnKeyDown
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: nChar, nRepCnt, nFlags
��    ��: ����
*/
void DownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
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
		//1.1. ��ǥ������ ���ϴ�.
		this->textEditor->destination = this->textEditor->x;
		//1.2. �̵��� ���¸� ��ġ��.
		this->textEditor->isMoving = true;
	}
	//3. �������� ���������� �ƴϸ�
	if (this->textEditor->note->GetCurrent() < this->textEditor->note->GetLength()) {
		//3.1. �����ٷ� �̵��ϴ�.
		row = this->textEditor->note->Next();
		this->textEditor->current = this->textEditor->note->GetAt(row - 1);
		//3.2. ��ǥ(X)�� ���ϴ�. 
		this->textEditor->x = 0;
		i = 0;
		while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
			this->textEditor->x += this->textEditor->current->GetAt(i)->GetWidth(this->textEditor->characterMetrics);
			i++;
		}
		//3.4. �����ٿ��� ĭ�� �̵��ϴ�.
		this->textEditor->current->Move(i);
	}
	
	//4. ���� ���� ���̿��� ���������̸�
	else {	
		//4.2. ���� ���� ������ ������ ���� �ƴϸ�
		if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength()) {
			//4.1. �޸� ������Ʈ�� �ȵ� �����̸� �޸���Ʈ�ѷ����� �����ϴ�.
			if (this->textEditor->isUpdated == false) {
				this->textEditor->memoryController->Save();
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			//4.2.1. �޸���Ʈ�ѷ����� �Ʒ��� �̵��ϴ�.
			this->textEditor->memoryController->MoveDown(this->textEditor->document->GetStart() + 1);
			this->textEditor->note->Move(this->textEditor->note->GetCloseRow(this->textEditor->note->GetCurrent()));
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			//4.2.3. X��ǥ�� ���ϴ�.
			this->textEditor->x = 0;
			i = 0;
			while (i < this->textEditor->current->GetLength() && this->textEditor->x < this->textEditor->destination) {
				this->textEditor->x += this->textEditor->current->GetAt(i)->GetWidth(this->textEditor->characterMetrics);
				i++;
			}
			//4.2.4. �ٿ��� �̵��ϴ�.
			this->textEditor->current->Move(i);
		}
		//4.3. ���� ���� ������ ������ ���̸�
		else {
			//4.3.1. �����ٿ��� ������ĭ���� �̵��ϴ�.
			this->textEditor->current->Last();
			//4.3.2. ��ǥ(X)�� ���ϴ�.
			this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
			this->textEditor->destination = this->textEditor->x;
		}
	}

	this->textEditor->y = this->textEditor->GetY();
	//5. ������Ʈ�ϴ�. (��ũ��, ĳ��)
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	//6. ȭ���� ������Ʈ�ϴ�.
	this->textEditor->Invalidate(FALSE);
}