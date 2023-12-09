//LButtonDownCommand.cpp
#include "LButtonDownCommand.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "Document.h"
#include "Glyph.h"
#include "Position.h"

LButtonDownCommand::LButtonDownCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

LButtonDownCommand::~LButtonDownCommand() {

}

void LButtonDownCommand::Execute() {
	::SetCapture(this->textEditor->m_hWnd);
	Long i = 0;
	Long below = this->textEditor->scrollController->below;
	Long hSPosition = this->textEditor->scrollController->horizontalScroll->GetPosition();
	Long vSPosition = this->textEditor->scrollController->verticalScroll->GetPosition();
	Long row = ((vSPosition + this->textEditor->point.y) / this->textEditor->characterMetrics->GetHeight()) + 1;
	Glyph* character = 0;

	this->textEditor->isClicking = true;

	if (GetKeyState(VK_SHIFT) & 0x8000) {
		Position previous;
		if (this->textEditor->isScrolling) {
			previous = Position(this->textEditor->row, this->textEditor->column);
		}
		else {
			previous = this->textEditor->document->GetPosition();
		}

		// ��
		if (row > below + this->textEditor->note->GetLength()) {
			row = below + this->textEditor->note->GetLength();
		}
		this->textEditor->note->Move(row - below);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->y = this->textEditor->GetY();

		// ĭ
		this->textEditor->x = 0;
		while (i < this->textEditor->current->GetLength() && this->textEditor->x < hSPosition + this->textEditor->point.x) {
			character = this->textEditor->current->GetAt(i);
			this->textEditor->x += character->GetWidth(this->textEditor->characterMetrics);
			i++;
		}

		// ĭ ��������
		if (i > 0 && i <= this->textEditor->current->GetLength() && this->textEditor->point.x + hSPosition != this->textEditor->x) {
			Long previous = this->textEditor->x - character->GetWidth(this->textEditor->characterMetrics);
			Long one = this->textEditor->point.x + hSPosition - previous;
			Long other = this->textEditor->x - (hSPosition + this->textEditor->point.x);

			if (one < other) {
				i--;
				this->textEditor->x = previous;
			}
		}
		this->textEditor->current->Move(i);
		Position current = this->textEditor->document->GetPosition();

		if (this->textEditor->isScrolling == false) {
			this->textEditor->document->Select(previous, current);
			this->textEditor->document->UpdateSelectingInfo(previous, current);
		}
		else {
			if (this->textEditor->document->isSelecting == false) {
				//previous�� ���� �ȿ� ���� ��
				if (previous.row >= this->textEditor->document->GetStart() && previous.row <= this->textEditor->document->GetStart() + this->textEditor->note->GetLength() - 1) {
					this->textEditor->document->Select(previous, current);
					this->textEditor->document->UpdateSelectingInfo(previous, current);
				}
				//previous�� ���� �ۿ� ���� �� //���� �� ���� ���ֱ�.
				else if (previous < current) {
					this->textEditor->document->startPosition = previous;
					this->textEditor->note->First();
					this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
					this->textEditor->current->Select();
					this->textEditor->current->First();
					previous = this->textEditor->document->GetPosition();
					this->textEditor->document->Move(current.row, current.column);
					this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
					this->textEditor->document->Select(previous, current);
					this->textEditor->document->endPosition = current;
					this->textEditor->document->isSelecting = true;
				}
				else if (previous > current) {
					this->textEditor->document->endPosition = previous;
					this->textEditor->note->Last();
					this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
					this->textEditor->current->Last();
					previous = this->textEditor->document->GetPosition();
					this->textEditor->document->Move(current.row, current.column);
					this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
					this->textEditor->document->Select(previous, current);
					this->textEditor->document->startPosition = current;
					this->textEditor->document->isSelecting = true;
				}
			}

			//���û����̸�
			else {
				//previous�� ���� �ȿ� ���� ��
				if (previous.row >= this->textEditor->document->GetStart() && previous.row <= this->textEditor->document->GetStart() + this->textEditor->note->GetLength() - 1) {
					this->textEditor->document->Select(previous, current);
					this->textEditor->document->UpdateSelectingInfo(previous, current);
				}

				//previous�� ���� �ۿ� ���� �� //���� �� ���� or �������� ���ֱ�.
				else if (previous < current) {
					if (previous == this->textEditor->document->endPosition) {
						this->textEditor->note->First();
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->current->Select();
						this->textEditor->current->First();
						previous = this->textEditor->document->GetPosition();
						this->textEditor->document->Move(current.row, current.column);
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->document->Select(previous, current);
						this->textEditor->document->UpdateSelectingInfo(previous, current);
					}
					else if (current < this->textEditor->document->endPosition) {
						this->textEditor->note->First();
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->current->UnSelect();
						this->textEditor->current->First();
						previous = this->textEditor->document->GetPosition();
						this->textEditor->document->Move(current.row, current.column);
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->document->Select(previous, current);
						this->textEditor->document->startPosition = current;
						this->textEditor->document->isSelecting = true;
					}
					else if (current == this->textEditor->document->endPosition) {
						this->textEditor->note->First();
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->current->UnSelect();
						this->textEditor->current->First();
						previous = this->textEditor->document->GetPosition();
						this->textEditor->document->Move(current.row, current.column);
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->document->Select(previous, current);
						this->textEditor->document->isSelecting = false;
					}
					else if (current > this->textEditor->document->endPosition) {
						this->textEditor->note->First();
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->current->Select();
						this->textEditor->current->First();
						previous = this->textEditor->document->GetPosition();
						this->textEditor->document->Move(current.row, current.column);
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->document->Select(previous, current);
						this->textEditor->document->startPosition = this->textEditor->document->endPosition;
						this->textEditor->document->endPosition = current;
						this->textEditor->document->isSelecting = true;
					}
				}
				else if (previous > current) {
					if (previous == this->textEditor->document->startPosition) {
						this->textEditor->note->Last();
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->current->Last();
						previous = this->textEditor->document->GetPosition();
						this->textEditor->document->Move(current.row, current.column);
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->document->Select(previous, current);
						this->textEditor->document->UpdateSelectingInfo(previous, current);
					}
					else if (current > this->textEditor->document->startPosition) {
						this->textEditor->note->Last();
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->current->Last();
						previous = this->textEditor->document->GetPosition();
						this->textEditor->document->Move(current.row, current.column);
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->document->Select(previous, current);
						this->textEditor->document->endPosition = current;
						this->textEditor->document->isSelecting = true;
					}
					else if (current == this->textEditor->document->startPosition) {
						this->textEditor->note->Last();
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->current->Last();
						previous = this->textEditor->document->GetPosition();
						this->textEditor->document->Move(current.row, current.column);
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->document->Select(previous, current);
						this->textEditor->document->isSelecting = false;
					}
					else if (current < this->textEditor->document->startPosition) {
						this->textEditor->note->Last();
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->current->Last();
						previous = this->textEditor->document->GetPosition();
						this->textEditor->document->Move(current.row, current.column);
						this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
						this->textEditor->document->Select(previous, current);
						this->textEditor->document->endPosition = this->textEditor->document->startPosition;
						this->textEditor->document->startPosition = current;
						this->textEditor->document->isSelecting = true;
					}
				}
			}
		}

		this->textEditor->isMoving = false;
		this->textEditor->isScrolling = false;
		this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
		this->textEditor->Notify();
		this->textEditor->Invalidate(FALSE);
	}

	else {
		//�������� �Լ��� ���� ��Ȳ�� ���� ��ġ�̵�
		if (this->textEditor->isScrolling && this->textEditor->document->isSelecting && this->textEditor->document->startPosition.row >= this->textEditor->document->GetStart() && this->textEditor->document->startPosition.row <= this->textEditor->document->GetEnd()) {
			this->textEditor->document->Move(this->textEditor->document->startPosition.row, this->textEditor->document->startPosition.column);
		}
		else if (this->textEditor->isScrolling && this->textEditor->document->isSelecting && this->textEditor->document->endPosition.row >= this->textEditor->document->GetStart() && this->textEditor->document->endPosition.row <= this->textEditor->document->GetEnd()) {
			this->textEditor->document->Move(this->textEditor->document->endPosition.row, this->textEditor->document->endPosition.column);
		}
		else if (this->textEditor->isScrolling && this->textEditor->document->isSelecting && this->textEditor->document->startPosition.row < this->textEditor->document->GetStart() && this->textEditor->document->endPosition.row > this->textEditor->document->GetEnd()) {
			this->textEditor->note->Last();
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
			this->textEditor->current->Last();
		}
		if (this->textEditor->document->isSelecting) {
			this->textEditor->note->UnSelect();
			this->textEditor->document->isSelecting = false;
		}

		// ��
		if (row > below + this->textEditor->note->GetLength()) {
			row = below + this->textEditor->note->GetLength();
		}
		this->textEditor->note->Move(row - below);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->y = this->textEditor->GetY();

		// ĭ
		this->textEditor->x = 0;
		while (i < this->textEditor->current->GetLength() && this->textEditor->x < hSPosition + this->textEditor->point.x) {
			character = this->textEditor->current->GetAt(i);
			this->textEditor->x += character->GetWidth(this->textEditor->characterMetrics);
			i++;
		}

		// ĭ ��������
		if (i > 0 && i <= this->textEditor->current->GetLength() && this->textEditor->point.x + hSPosition != this->textEditor->x) {
			Long previous = this->textEditor->x - character->GetWidth(this->textEditor->characterMetrics);
			Long one = this->textEditor->point.x + hSPosition - previous;
			Long other = this->textEditor->x - (hSPosition + this->textEditor->point.x);

			if (one < other) {
				i--;
				this->textEditor->x = previous;
			}
		}
		this->textEditor->current->Move(i);
		this->textEditor->isMoving = false;
		this->textEditor->isScrolling = false;
		this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
		this->textEditor->Notify();
		this->textEditor->Invalidate(FALSE);
	}
}

void LButtonDownCommand::Unexecute() {

}