//ScrollController.cpp
#include "ScrollController.h"
#include "Subject.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "Scroll.h"
#include "HorizontalScroll.h"
#include "VerticalScroll.h"
#include "CharacterMetrics.h"
#include "ReplaceInfoStack.h"
#include "Position.h"
#include "Document.h"
#include "FileMaker.h"
#include "resource.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
클래스명: ScrollController
기    능: 스크롤이 움직일 때마다 스크롤 위치를 업데이트하다.
*/

ScrollController::ScrollController(TextEditor* textEditor) {
	this->textEditor = textEditor;
	dynamic_cast<Subject*>(this->textEditor)->Attach(this);

	this->horizontalScroll = new HorizontalScroll;
	this->verticalScroll = new VerticalScroll;
	this->vaildMin = 0;
	this->diskFileVSMax = 0;
	this->vSMax = 0;
	this->below = 0;
}

ScrollController::~ScrollController() {
	dynamic_cast<Subject*>(this->textEditor)->Detach(this);

	if (this->horizontalScroll != 0 && this->verticalScroll !=0) {
		delete this->horizontalScroll;
		delete this->verticalScroll;
	}
}

void ScrollController::Update(Subject* subject) {
	Long minimum = 0;
	Long maximum = 0;
	Long page;
	Long position;
	SCROLLINFO si;
	
	maximum = this->horizontalScroll->GetMaximum();
	page = this->textEditor->rect.right;
	if (maximum < page) {
		this->horizontalScroll->SetPosition(0);
	}
	position = this->horizontalScroll->GetPosition();
	if (this->horizontalScroll != 0) {
		delete this->horizontalScroll;
		this->horizontalScroll = 0;
	}
	if (this->horizontalScroll == 0) {
		this->horizontalScroll = new HorizontalScroll(minimum, maximum, page, position);
	}
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	si.nMin = this->horizontalScroll->GetMinimum();
	si.nMax = this->horizontalScroll->GetMaximum();
	si.nPage = this->horizontalScroll->GetPage();
	si.nPos = this->horizontalScroll->GetPosition();
	SetScrollInfo(this->textEditor->m_hWnd, SB_HORZ, &si, TRUE);

	//수직스크롤
	minimum = 0;
	maximum = this->verticalScroll->GetMaximum();
	page = this->textEditor->rect.bottom;
	if (maximum < page) {
		this->verticalScroll->SetPosition(0);
	}
	position = this->verticalScroll->GetPosition();

	if (this->verticalScroll != 0) {
		delete this->verticalScroll;
		this->verticalScroll = 0;
	}
	if (this->verticalScroll == 0) {
		this->verticalScroll = new VerticalScroll(minimum, maximum, page, position);
	}
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = this->verticalScroll->GetMinimum();
	si.nMax = this->verticalScroll->GetMaximum();
	si.nPage = this->verticalScroll->GetPage();
	si.nPos = this->verticalScroll->GetPosition();
	SetScrollInfo(this->textEditor->m_hWnd, SB_VERT, &si, TRUE);
}

void ScrollController::UpdateMaximum() {
	this->UpdateHSMax();
	this->UpdateVSMax();
}

void ScrollController::UpdateHSMax() {
	Long i;
	Long maximum = 0;
	Long rowLength;
	FILE* file = 0;
	FileMaker fm(this->textEditor);

	switch (this->textEditor->scrollFlags) {

		case SF_NORMAL:
			maximum = this->horizontalScroll->GetMaximum();
			break;

		case SF_ALL:
			i = 0;
			while (i < this->textEditor->note->GetLength()) {
				rowLength = this->textEditor->note->GetAt(i)->GetRowLength();
				if (rowLength > maximum || i == 0) {
					maximum = rowLength;
					this->textEditor->longestRow = this->textEditor->document->GetStart() + i;
				}
				i++;
			}
			file = fopen(fm.GetFile(), "rt");
			if (file != 0) {
				i = 1;
				while (i < this->textEditor->document->GetStart()) {
					fseek(file, (i - 1) * (12 + MAX) + 5, SEEK_SET);
					fscanf(file, "%d", &rowLength);
					if (rowLength > maximum) {
						maximum = rowLength;
						this->textEditor->longestRow = i;
					}
					i++;
				}

				i = this->textEditor->document->GetEnd() + 1;
				while (i <= this->textEditor->document->GetLength()) {
					fseek(file, (i - 1) * (12 + MAX) + 5, SEEK_SET);
					fscanf(file, "%d", &rowLength);
					if (rowLength > maximum) {
						maximum = rowLength;
						this->textEditor->longestRow = i;
					}
					i++;
				}
				fclose(file);
			}
			break;

		case SF_COMPARE:
			maximum = this->horizontalScroll->GetMaximum();
			if (this->textEditor->current->GetRowLength() > maximum) {
				maximum = this->textEditor->current->GetRowLength();
				this->textEditor->longestRow = this->textEditor->document->GetStart() + this->textEditor->note->GetCurrent() - 1;
			}
			break;

		case SF_WRAP:
			maximum = this->textEditor->rect.right - 1;
			this->textEditor->longestRow = 0;
			break;

		default: break;

	}
	this->horizontalScroll->SetMaximum(maximum);
}

void ScrollController::UpdateVSMax() {

	if (this->textEditor->isUpdated == false) {
		this->vSMax = this->diskFileVSMax + (this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight());
	}
	this->verticalScroll->SetMaximum(this->vSMax);
}

void ScrollController::UpdateFileVSInfo(bool needUpdateVaildMin) {
	char buffer[MAX];
	Long i;
	Long j;
	Long sum;
	Long width;
	Long length;
	Long characterWidth;
	FILE* file;
	FileMaker fm(this->textEditor);

	if (this->textEditor->isWrapped) {
		file = fopen(fm.GetFile(), "rt");
		if (file != NULL) {
			if (needUpdateVaildMin) {
				this->UpdateVaildMin();
			}
			sum = this->below;
			i = this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount();
			while (i <= this->textEditor->document->GetLength()) {
				sum++;
				width = this->textEditor->rect.right;
				fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
				fscanf(file, "%d", &length);
				fseek(file, 8, SEEK_CUR);
				fread(buffer, length, 1, file);
				j = 0;
				while (j < length) {
					if (buffer[j] >= 0 && buffer[j] < ASCII) {
						characterWidth = this->textEditor->characterMetrics->GetWidth(buffer[j]);
					}
					else {
						characterWidth = this->textEditor->characterMetrics->GetWidth(128);
						j++;
					}
					width -= characterWidth;
					if (width < 0) {
						sum++;
						width = this->textEditor->rect.right;
						width -= characterWidth;
					}
					j++;
				}
				i++;
			}
			fclose(file);
		}
	}
	else {
		this->below = this->textEditor->document->GetStart() - 1;
		this->vaildMin = this->below * this->textEditor->characterMetrics->GetHeight();
		if (this->textEditor->document->GetLength() > 0) {
			sum = this->textEditor->document->GetLength() - (this->textEditor->note->GetRowCount());
		}
	}
	this->diskFileVSMax = sum * this->textEditor->characterMetrics->GetHeight();
	this->vSMax = this->diskFileVSMax + (this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight());
}

Long ScrollController::UpdateVaildMin() {
	char buffer[MAX];
	Long i = 1;
	Long j;
	Long width;
	Long characterWidth;
	Long length;
	FILE* file;
	FileMaker fm(this->textEditor);

	if (this->textEditor->isWrapped) {
		file = fopen(fm.GetFile(), "rt");
		if (file != NULL) {
			this->below = 0;
			while (i < this->textEditor->document->GetStart()) {
				this->below++;
				width = this->textEditor->rect.right;
				fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
				fscanf(file, "%d", &length);
				fseek(file, 8, SEEK_CUR);
				fread(buffer, length, 1, file);
				j = 0;
				while (j < length) {
					if (buffer[j] >= 0 && buffer[j] < ASCII) {
						characterWidth = this->textEditor->characterMetrics->GetWidth(buffer[j]);
					}
					else {
						characterWidth = this->textEditor->characterMetrics->GetWidth(128);
						j++;
					}
					if (width - characterWidth >= 0) {
						width -= characterWidth;
					}
					else {
						this->below++;
						width = this->textEditor->rect.right;
						width -= characterWidth;
					}
					j++;
				}
				i++;
			}
			fclose(file);
		}
	}
	else {
		this->below = this->textEditor->document->GetStart() - 1;
	}
	this->vaildMin = this->below * this->textEditor->characterMetrics->GetHeight();

	return this->vaildMin;
}

void ScrollController::MoveUp(Long count) {
	this->below -= count;
	this->vaildMin -= count * this->textEditor->characterMetrics->GetHeight();
}

void ScrollController::MoveDown(Long count) {
	this->below += count;
	this->vaildMin += count * this->textEditor->characterMetrics->GetHeight();
}

void ScrollController::UpdatePosition(CharacterMetrics* characterMetrics) {
	Long x;
	Long maximum = 0;
	Long position;
	Long page;

	position = this->horizontalScroll->GetPosition();
	maximum = this->horizontalScroll->GetMaximum();
	page = this->textEditor->rect.right;
	if (maximum - page >= 0 && position > maximum - page) {
		position = maximum - page;
		this->horizontalScroll->SetPosition(position);
	}
	if (this->textEditor->x < position + characterMetrics->GetWidth(32)) {
		position = this->textEditor->x - characterMetrics->GetWidth(32);
		if (position < 0) {
			position = 0;
		}
	}
	else if (this->textEditor->isComposing == true &&
		this->textEditor->x + characterMetrics->GetWidth(128) > position + page - characterMetrics->GetWidth(32)) {
		position = this->textEditor->x + characterMetrics->GetWidth(128) + characterMetrics->GetWidth(32) - page;
		if (position > maximum - page) {
			position = maximum - page;
		}
	}
	else if (this->textEditor->x > position + page - characterMetrics->GetWidth(32)) {
		position = this->textEditor->x + characterMetrics->GetWidth(32) - page;
		if (position > maximum - page) {
			position = maximum - page;
			if (position < 0) {
				position = 0;
			}
		}
	}
	this->horizontalScroll->SetPosition(position);

	// 수직스크롤
	position = this->verticalScroll->GetPosition();
	maximum = this->verticalScroll->GetMaximum();
	page = this->textEditor->rect.bottom;
	if (this->textEditor->y < position) {
		position = this->textEditor->y;
	}
	else if (this->textEditor->y + this->textEditor->characterMetrics->GetHeight() > position + page) {
		position = this->textEditor->y + this->textEditor->characterMetrics->GetHeight() - page;
	}
	if (maximum < page) {
		position = 0;
	}
	else if (maximum - page >= 0 && position > maximum - page) {
		position = maximum - page;
	}
	this->verticalScroll->SetPosition(position);
}

int ScrollController::LineLeft(int distance) {
	int position;
	
	position = this->horizontalScroll->GetPosition();
	position -= distance;
	
	if (position < 0) {
		position = 0;
	}
	
	this->horizontalScroll->SetPosition(position);

	return position;
}

int ScrollController::LineRight(int distance) {
	int position;

	position = this->horizontalScroll->GetPosition();
	position += distance;

	if (position > this->horizontalScroll->GetMaximum() - this->horizontalScroll->GetPage()) {
		position = this->horizontalScroll->GetMaximum() - this->horizontalScroll->GetPage();
	}

	this->horizontalScroll->SetPosition(position);

	return position;
}

int ScrollController::PageLeft(int page) {
	int position;
	
	position = this->horizontalScroll->GetPosition();
	position -= page;
	
	if (position < 0) {
		position = 0;
	}
	
	this->horizontalScroll->SetPosition(position);

	return position;
}

int ScrollController::PageRight(int page) {
	int position;

	position = this->horizontalScroll->GetPosition();
	position += page;
	
	if (position > this->horizontalScroll->GetMaximum() - this->horizontalScroll->GetPage()) {
		position = this->horizontalScroll->GetMaximum() - this->horizontalScroll->GetPage();
	}

	this->horizontalScroll->SetPosition(position);

	return position;
}

int ScrollController::LineDown(int distance) {
	int position;
	Long max = this->verticalScroll->GetMaximum() - this->verticalScroll->GetPage();

	if (max < 0) {
		max = 0;
	}
	position = this->verticalScroll->GetPosition();
	position += distance;
	if (position > max) {
		position = max;
	}
	this->verticalScroll->SetPosition(position);
	
	return position;
}

int ScrollController::LineUp(int distance) {
	int position;

	position = this->verticalScroll->GetPosition();
	position -= distance;

	if (position < 0) {
		position = 0;
	}

	this->verticalScroll->SetPosition(position);

	return position;
}

int ScrollController::PageDown(int page) {
	int position;

	position = this->verticalScroll->GetPosition();
	position += page;

	if (position > this->verticalScroll->GetMaximum() - this->verticalScroll->GetPage()) {
		position = this->verticalScroll->GetMaximum() - this->verticalScroll->GetPage();
	}

	this->verticalScroll->SetPosition(position);

	return position;
}

int ScrollController::PageUp(int page) {
	int position;

	position = this->verticalScroll->GetPosition();
	
	position -= page;

	if (position < 0) {
		position = 0;
	}

	this->verticalScroll->SetPosition(position);

	return position;
}

int ScrollController::HSThumbTrack(int position) {
	
	if (position > this->horizontalScroll->GetMaximum() - this->horizontalScroll->GetPage()) {
		position = this->horizontalScroll->GetMaximum() - this->horizontalScroll->GetPage();
	}

	this->horizontalScroll->SetPosition(position);

	return position;
}

int ScrollController::VSThumbTrack(int position) {

	if (position < 0) {
		position = 0;
	}

	else if (position > this->verticalScroll->GetMaximum() - this->verticalScroll->GetPage()) {
		position = this->verticalScroll->GetMaximum() - this->verticalScroll->GetPage();
	}

	this->verticalScroll->SetPosition(position);

	return position;
}

Long ScrollController::SetVaildMin(Long vaildMin) {
	this->vaildMin = vaildMin;
	return this->vaildMin;
}

Long ScrollController::SetBelow(Long below) {
	this->below = below;
	return this->below;
}