//MouseMoveCommand.cpp
#include "MouseMoveCommand.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "Document.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "Position.h"
#include "Glyph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif
MouseMoveCommand::MouseMoveCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

MouseMoveCommand::~MouseMoveCommand() {

}

void MouseMoveCommand::Execute() {
	Long row;
	Long below;
	Long vaildMin;
	Long vaildMax;
	Long start = this->textEditor->document->GetStart();
	Long end = this->textEditor->document->GetEnd();
	Long length = this->textEditor->document->GetLength();
	Long height = this->textEditor->characterMetrics->GetHeight();
	Long HSP = this->textEditor->scrollController->horizontalScroll->GetPosition();
	Long VSP = this->textEditor->scrollController->verticalScroll->GetPosition();
	Position previous = this->textEditor->document->GetPosition(); // isScrolling상황이라면 어떨지 생각해보자.
	Position current;
	Glyph* character = 0;

	if (this->textEditor->isUpdated == false && this->textEditor->point.y <= 0 && this->textEditor->point.y >= this->textEditor->rect.bottom) {
		this->textEditor->memoryController->Save();
		this->textEditor->scrollController->UpdateFileVSInfo(true);
		length = this->textEditor->document->GetLength();
		end = this->textEditor->document->SetEnd(start + this->textEditor->note->GetRowCount() - 1);
	}
	vaildMin = this->textEditor->scrollController->GetVaildMin();
	while (start > 1 && this->textEditor->point.y + VSP < vaildMin) {
		this->textEditor->memoryController->MoveUp(start - 1);
		start = this->textEditor->document->GetStart();
		end = start + this->textEditor->note->GetRowCount() - 1;
		vaildMin = this->textEditor->scrollController->GetVaildMin();
	}
	vaildMax = vaildMin + this->textEditor->note->GetLength() * height;
	while (end < length && this->textEditor->point.y + VSP > vaildMax) {
		this->textEditor->memoryController->MoveDown(start + 1);
		start = this->textEditor->document->GetStart();
		end = start + this->textEditor->note->GetRowCount() - 1;
		vaildMin = this->textEditor->scrollController->GetVaildMin();
		vaildMax = vaildMin + this->textEditor->note->GetLength() * height;
	}
	below = this->textEditor->scrollController->below;
	row = (this->textEditor->point.y + VSP) / height + 1 - below;
	if (row < 1) {
		row = 1;
	}
	if (row > this->textEditor->note->GetLength()) {
		row = this->textEditor->note->GetLength();
	}
	this->textEditor->note->Move(row);
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->current->Move(HSP + this->textEditor->point.x, this->textEditor->characterMetrics);

#if 0
	//1.1. 클릭위치 Y축좌표 + 수직스크롤 포지션이 현재 Y축 위치보다 작거나 클릭위치 Y축좌표 + 수직스크롤 포지션이 다음 줄 Y축 위치보다 크거나 같으면
	if (this->textEditor->point.y + vSPosition < this->textEditor->y || this->textEditor->point.y + vSPosition >= this->textEditor->y + this->textEditor->characterMetrics->GetHeight()) {
		row = (this->textEditor->point.y + vSPosition) / this->textEditor->characterMetrics->GetHeight() + 1;
		if (row > below + this->textEditor->note->GetLength()) {
			row = below + this->textEditor->note->GetLength();
		}
		this->textEditor->note->Move(row - below);
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->y = this->textEditor->GetY();

	}
	//1.2. 현재 칸을 찾다.
	i = 0;
	this->textEditor->x = 0;
	while (i < this->textEditor->current->GetLength() && this->textEditor->x < hSPosition + this->textEditor->point.x) {
		character = this->textEditor->current->GetAt(i);
		this->textEditor->x += character->GetWidth(this->textEditor->characterMetrics);
		i++;
	}
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
#endif
	current = Position(this->textEditor->document->GetRowNumber(), this->textEditor->note->GetColumnNumber());
	if (previous != current) {
		this->textEditor->document->Select(previous, current);
		this->textEditor->document->UpdateSelectingInfo(previous, current);
	}
	this->textEditor->isContinuous = false;
	this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent());
	this->textEditor->y = this->textEditor->GetY();
	this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
#if 0
	TRACKMOUSEEVENT tme;
	::ZeroMemory(&tme, sizeof(TRACKMOUSEEVENT));
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = this->textEditor->m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = 1;
	_TrackMouseEvent(&tme);
#endif
}

void MouseMoveCommand::Unexecute() {

}