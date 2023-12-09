//MemoryController.cpp
#include "MemoryController.h"
#include "TextEditor.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "CharacterMetrics.h"
#include "resource.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "Document.h"
#include "FileMaker.h"
#include "DummyRow.h"
#include "resource.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

MemoryController::MemoryController(TextEditor* textEditor) {
	this->textEditor = textEditor;
	dynamic_cast<Subject*>(this->textEditor)->Attach(this);
}

MemoryController::~MemoryController() {
	dynamic_cast<Subject*>(this->textEditor)->Detach(this);
}

void MemoryController::Update(Subject* subject) {
	int minimum = (this->textEditor->rect.bottom - 1) / this->textEditor->characterMetrics->GetHeight() + 1;
	if ((this->textEditor->rect.bottom - 1) % this->textEditor->characterMetrics->GetHeight() > 0) {
		minimum++;
	}
	Long position = this->textEditor->scrollController->verticalScroll->GetPosition();
	Long page = this->textEditor->rect.bottom;
	Long vaildMin;
	Long vaildMax;
	
	if (this->textEditor->note->GetRowCount() < minimum) {
		if (this->textEditor->isUpdated == false) {
			this->Save();
			this->textEditor->document->SetEnd(this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1);
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		this->TakeIn(minimum);
	}

	if (this->textEditor->note->GetRowCount() > MAXROW) {
		this->Save();
		this->textEditor->document->SetEnd(this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1);
		this->textEditor->scrollController->UpdateFileVSInfo(true);
		this->TakeOut();
	}

	vaildMin = this->textEditor->scrollController->GetVaildMin();
	vaildMax = vaildMin + this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight() - page;
	if (vaildMax < 0) {
		vaildMax = 0;
	}
	if (position < vaildMin || position > vaildMax) {
		if (this->textEditor->isUpdated == false) {
			this->Save();
			this->textEditor->scrollController->UpdateFileVSInfo(true);
		}
		Position current(this->textEditor->document->GetRowNumber(), this->textEditor->note->GetColumnNumber());
		if (position < vaildMin) {
			if (this->textEditor->isWrapped == true) {
				this->ScrollUp(true, position, page, vaildMin, vaildMax);
			}
			else {
				this->ScrollUp(position, page, vaildMin, vaildMax);
			}
		}
		else {
			if (this->textEditor->isWrapped == true) {
				this->ScrollDown(true, position, page, vaildMin, vaildMax);
			}
			else {
				this->ScrollDown(position, page, vaildMin, vaildMax);
			}
		}
		//확대 축소로 인해 ScrollUp / Down이 호출 되었을 때 원래 위치로 돌아가도록 처리.
		if (this->textEditor->isScrolling == false && current.row >= this->textEditor->document->GetStart() && current.row <= this->textEditor->document->GetEnd()) {
			this->textEditor->document->Move(current.row, current.column);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
	}
}

void MemoryController::Save() {
	char buffer[MAX];
	char contents[MAX - 2];
	Long i;
	Long length;
	Long rowLength;
	Long count = 0;
	FILE* file;
	FILE* temp;
	FileMaker fm(this->textEditor);

	file = fopen(fm.GetFile(), "rt");
	temp = fopen(fm.GetTemp(), "wt");
	if (file != NULL && temp != NULL) {
		i = 1;
		while (i < this->textEditor->document->GetStart()) {
			fgets(buffer, MAX, file);
			fputs(buffer, temp);
			fgets(buffer, MAX, file);
			fputs(buffer, temp);
			i++;
		}
		count = i - 1;
		i = 0;
		while (i < this->textEditor->note->GetLength()) {
			this->textEditor->note->ConvertChar(&i, &length, &rowLength, contents);
			fprintf(temp, "%04d %05d\n", length, rowLength);
			fwrite(contents, MAX - 2, 1, temp);
			if (i < this->textEditor->note->GetLength()) {
				fputs("\n", temp);
			}
			count++;
		}
		i = this->textEditor->document->GetEnd() + 1;
		if (i <= this->textEditor->document->GetLength()) {
			fputs("\n", temp);
		}
		fseek(file, ((i - 1) * (12 + MAX)), SEEK_SET);
		while (i <= this->textEditor->document->GetLength()) {
			fgets(buffer, MAX, file);
			fputs(buffer, temp);
			fgets(buffer, MAX, file);
			fputs(buffer, temp);
			count++;
			i++;
		}
		fclose(file);
		fclose(temp);
	}
	remove(fm.GetFile());
	rename(fm.GetTemp(), fm.GetFile());
	this->textEditor->document->SetLength(count);
	this->textEditor->isUpdated = true;
}

void MemoryController::SaveRowLength() {
	char buffer[MAX];
	char contents[MAX - 2];
	Long i;
	Long j;
	Long count;
	Long length;
	Long rowLength;
	FILE* file;
	FILE* temp;
	FileMaker fm(this->textEditor);

	file = fopen(fm.GetFile(), "rt");
	temp = fopen(fm.GetTemp(), "wt");
	if (file != NULL && temp != NULL) {
		i = 1;
		while (i < this->textEditor->document->GetStart()) {
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &length);
			fseek(file, 8, SEEK_CUR);
			fgets(buffer, MAX, file);
			rowLength = 0;
			j = 0;
			while (j < length) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					rowLength += this->textEditor->characterMetrics->GetWidth(buffer[j]);
				}
				else {
					rowLength += this->textEditor->characterMetrics->GetWidth(128);
					j++;
				}
				j++;
			}
			fprintf(temp, "%04d %05d\n", length, rowLength);
			fputs(buffer, temp);
			i++;
		}
		count = i - 1;
		i = 0;
		while (i < this->textEditor->note->GetLength()) {
			this->textEditor->note->ConvertChar(&i, &length, &rowLength, contents, this->textEditor->characterMetrics);
			fprintf(temp, "%04d %05d\n", length, rowLength);
			fwrite(contents, MAX - 2, 1, temp);
			if (i < this->textEditor->note->GetLength()) {
				fputs("\n", temp);
			}
			count++;
		}
		i = this->textEditor->document->GetEnd() + 1;
		if (i <= this->textEditor->document->GetLength()) {
			fputs("\n", temp);
		}
		while (i <= this->textEditor->document->GetLength()) {
			fseek(file, ((i - 1) * (12 + MAX)), SEEK_SET);
			fscanf(file, "%d", &length);
			fseek(file, 8, SEEK_CUR);
			fgets(buffer, MAX, file);
			rowLength = 0;
			j = 0;
			while (j < length) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					rowLength += this->textEditor->characterMetrics->GetWidth(buffer[j]);
				}
				else {
					rowLength += this->textEditor->characterMetrics->GetWidth(128);
					j++;
				}
				j++;
			}
			fprintf(temp, "%04d %05d\n", length, rowLength);
			fputs(buffer, temp);
			count++;
			i++;
		}
		fclose(file);
		fclose(temp);
	}
	remove(fm.GetFile());
	rename(fm.GetTemp(), fm.GetFile());
	this->textEditor->document->SetLength(count);
	this->textEditor->isUpdated = true;
}

void MemoryController::SaveRowLength(bool isWrapped) {
	char buffer[MAX];
	Long i = 1;
	Long j;
	Long k;
	Long byte;
	Long rowLength;
	FILE* file;
	Glyph* row;
	Glyph* character;
	FileMaker fm(this->textEditor);

	file = fopen(fm.GetFile(), "rt+");
	if (file != NULL) {
		while (i < this->textEditor->document->GetStart()) {
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			buffer[byte] = '\0';
			rowLength = 0;
			j = 0;
			while (j < byte) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					rowLength += this->textEditor->characterMetrics->GetWidth(buffer[j]);
				}
				else {
					rowLength += this->textEditor->characterMetrics->GetWidth(128);
					j++;
				}
				j++;
			}
			fseek(file, (i - 1) * (12 + MAX) + 5, SEEK_SET);
			fprintf(file, "%05d", rowLength);
			i++;
		}
		i = 1;
		while (i <= this->textEditor->note->GetRowCount()) {
			j = i;
			rowLength = 0;
			row = this->textEditor->note->GetAt(j - 1);
			dynamic_cast<Row*>(row)->rowLength = 0;
			k = 0;
			while (k < row->GetLength()) {
				character = row->GetAt(k);
				dynamic_cast<Row*>(row)->rowLength += character->GetWidth(this->textEditor->characterMetrics);
				k++;
			}
			rowLength += dynamic_cast<Row*>(row)->rowLength;
			j++;
			while (j < this->textEditor->note->GetLength() && dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(j - 1))) {
				row = this->textEditor->note->GetAt(j - 1);
				dynamic_cast<Row*>(row)->rowLength = 0;
				k = 0;
				while (k < row->GetLength()) {
					character = row->GetAt(k);
					dynamic_cast<Row*>(row)->rowLength += character->GetWidth(this->textEditor->characterMetrics);
					k++;
				}
				rowLength += dynamic_cast<Row*>(row)->rowLength;
				j++;
			}
			fseek(file, (i - 1 + this->textEditor->document->GetStart() - 1) * (12 + MAX) + 5, SEEK_SET);
			fprintf(file, "%05d", rowLength);
			i++;
		}
		i = this->textEditor->document->GetEnd() + 1;
		while (i <= this->textEditor->document->GetLength()) {
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			buffer[byte] = '\0';
			rowLength = 0;
			j = 0;
			while (j < byte) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					rowLength += this->textEditor->characterMetrics->GetWidth(buffer[j]);
				}
				else {
					rowLength += this->textEditor->characterMetrics->GetWidth(128);
					j++;
				}
				j++;
			}
			fseek(file, (i - 1) * (12 + MAX) + 5, SEEK_SET);
			fprintf(file, "%05d", rowLength);
			i++;
		}
		fclose(file);
	}
	this->textEditor->isUpdated = true;
}

void MemoryController::TakeIn(int minimum)
{
	Long start;
	Long end;
	Long count;
	Long vaildMin;
	Long current = this->textEditor->note->GetCurrent();
	FILE* file;
	FileMaker fm(this->textEditor);

	file = fopen(fm.GetFile(), "rt");
	if (file != NULL) {
		if (this->textEditor->document->GetEnd() < this->textEditor->document->GetLength()) {
			count = this->textEditor->note->GetLength();
			start = this->textEditor->document->GetEnd() + 1;
			end = minimum - this->textEditor->note->GetRowCount() - 1 + start;
			if (end > this->textEditor->document->GetLength()) {
				end = this->textEditor->document->GetLength();
			}
			if (this->textEditor->isWrapped) {
				this->textEditor->document->LoadToMemory(start, end, this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
			}
			else {
				this->textEditor->document->LoadToMemory(start, end, this->textEditor->characterMetrics, &fm);
			}
			this->textEditor->document->SetEnd(end);
			count = this->textEditor->note->GetLength() - count;
			this->textEditor->scrollController->diskFileVSMax -= count * this->textEditor->characterMetrics->GetHeight();
		}
		else if (this->textEditor->note->GetRowCount() < minimum && this->textEditor->document->GetStart() > 1) {
			count = this->textEditor->note->GetLength();
			end = this->textEditor->document->GetStart() - 1;
			start = end - (minimum - this->textEditor->note->GetRowCount()) + 1;
			if (start < 1) {
				start = 1;
			}
			if (this->textEditor->isWrapped) {
				this->textEditor->document->LoadToMemory(true, start, end, this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
			}
			else {
				this->textEditor->document->LoadToMemory(true, start, end, this->textEditor->characterMetrics, &fm);
			}
			count = this->textEditor->note->GetLength() - count;
			current += count;
			this->textEditor->document->SetStart(start);
			this->textEditor->scrollController->below -= count;
			vaildMin = this->textEditor->scrollController->below * this->textEditor->characterMetrics->GetHeight();
			this->textEditor->scrollController->SetVaildMin(vaildMin);
			this->textEditor->scrollController->diskFileVSMax -= count * this->textEditor->characterMetrics->GetHeight();
		}
		fclose(file);
	}
	this->textEditor->note->Move(current);
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
}

void MemoryController::TakeOut() {
	Long i;
	Long count = 0;
	Long current = this->textEditor->note->GetCurrent();
	Long start = this->textEditor->document->GetStart();
	Long end;
	Long vaildMin;
	Long vSPosition = this->textEditor->scrollController->verticalScroll->GetPosition();
	Glyph* row;

	i = vSPosition / this->textEditor->characterMetrics->GetHeight() + 1;
	i -= this->textEditor->scrollController->GetBelow();
	row = this->textEditor->note->GetAt(i - 1);
	while (dynamic_cast<DummyRow*>(row)) {
		i--;
		row = this->textEditor->note->GetAt(i - 1);
	}
	while (1 < i && this->textEditor->note->GetRowCount() > OPTIMAL) {
		this->textEditor->note->Remove(0);
		row = this->textEditor->note->GetAt(0);
		while (dynamic_cast<DummyRow*>(row)) {
			this->textEditor->note->Remove(0);
			count++;
			this->textEditor->scrollController->below++;
			current--;
			i--;
			row = this->textEditor->note->GetAt(0);
		}
		count++;
		this->textEditor->scrollController->below++;
		current--;
		start++;
		i--;
	}
	while (this->textEditor->note->GetRowCount() > OPTIMAL) {
		count++;
		this->textEditor->note->Remove(this->textEditor->note->GetLength() - 1);
	}
	end = start + this->textEditor->note->GetRowCount() - 1;
	vaildMin = this->textEditor->scrollController->below * this->textEditor->characterMetrics->GetHeight();
	this->textEditor->note->Move(current);
	this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
	this->textEditor->document->SetStart(start);
	this->textEditor->document->SetEnd(end);
	this->textEditor->scrollController->SetVaildMin(vaildMin);
	this->textEditor->scrollController->diskFileVSMax += (count * this->textEditor->characterMetrics->GetHeight());
}

void MemoryController::MoveUp(Long rowNumber) {
	Long i = 1;
	Long add = 0;
	Long count;
	FileMaker fm(this->textEditor);

	count = this->textEditor->document->GetStart() - rowNumber;
	while (this->textEditor->note->GetLength() > 0 && i <= count) {
		while (dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(this->textEditor->note->GetLength() - 1))) {
			this->textEditor->note->Remove(this->textEditor->note->GetLength() - 1);
		}
		this->textEditor->note->Remove(this->textEditor->note->GetLength() - 1);
		i++;
	}
	add = this->textEditor->note->GetLength();
	i--;
	if (this->textEditor->isWrapped) {
		this->textEditor->document->LoadToMemory(true, rowNumber, rowNumber + i - 1, this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
	}
	else {
		this->textEditor->document->LoadToMemory(true, rowNumber, rowNumber + i - 1, this->textEditor->characterMetrics, &fm);
	}
	add = this->textEditor->note->GetLength() - add;
	this->textEditor->document->SetStart(rowNumber);
	this->textEditor->document->SetEnd(rowNumber + this->textEditor->note->GetRowCount() - 1);
	if (this->textEditor->document->isSelecting == true &&
		rowNumber <= this->textEditor->document->endPosition.row && rowNumber + i - 1 >= this->textEditor->document->startPosition.row) {
		this->textEditor->document->SelectLoadDatas(rowNumber, rowNumber + i - 1);
	}
	if (count <= this->textEditor->note->GetRowCount()) {
		this->textEditor->scrollController->MoveUp(add);
	}
	else {
		this->textEditor->scrollController->UpdateVaildMin();
	}
	if (this->textEditor->isWrapped) {
		this->textEditor->scrollController->diskFileVSMax = this->textEditor->scrollController->vSMax - (this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight());
	}
	else {
		this->textEditor->scrollController->diskFileVSMax = (this->textEditor->document->GetLength() - this->textEditor->note->GetLength()) * this->textEditor->characterMetrics->GetHeight();
	}
}

void MemoryController::MoveDown(Long rowNumber) {
	Long i = 1;
	Long remove = 0;
	Long count;
	Long start;
	Long end;
	FileMaker fm(this->textEditor);

	count = rowNumber - this->textEditor->document->GetStart();
	while (i <= count && this->textEditor->note->GetLength() > 0) {
		this->textEditor->note->Remove(0);
		remove++;
		while (this->textEditor->note->GetLength() > 0 && dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(0))) {
			this->textEditor->note->Remove(0);
			remove++;
		}
		i++;
	}
	i--;
	start = rowNumber + this->textEditor->note->GetRowCount();
	end = start + i -1;
	if (this->textEditor->isWrapped == true) {
		this->textEditor->document->LoadToMemory(start, end, this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
	}
	else {
		this->textEditor->document->LoadToMemory(start, end, this->textEditor->characterMetrics, &fm);
	}
	this->textEditor->document->SetStart(rowNumber);
	this->textEditor->document->SetEnd(rowNumber + this->textEditor->note->GetRowCount() - 1);
	if (this->textEditor->document->isSelecting == true && start <= this->textEditor->document->endPosition.row && end >= this->textEditor->document->startPosition.row) {
		this->textEditor->document->SelectLoadDatas(start, end);
	}
	if (count <= this->textEditor->note->GetRowCount()) {
		this->textEditor->scrollController->MoveDown(remove);
	}
	else {
		this->textEditor->scrollController->UpdateVaildMin();
	}
	if (this->textEditor->isWrapped) {
		this->textEditor->scrollController->diskFileVSMax = this->textEditor->scrollController->vSMax - (this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight());
	}
	else {
		this->textEditor->scrollController->diskFileVSMax = (this->textEditor->document->GetLength() - this->textEditor->note->GetLength()) * this->textEditor->characterMetrics->GetHeight();
	}
}

void MemoryController::ScrollUp(Long position, Long page, Long vaildMin, Long vaildMax){
	Long start;
	Long end;
	Long i = 1;
	Long distance = this->textEditor->scrollController->GetVaildMin() - position;
	Long rowCount = this->textEditor->note->GetRowCount();
	Long count = distance / this->textEditor->characterMetrics->GetHeight();
	FileMaker fm(this->textEditor);

	if (distance % this->textEditor->characterMetrics->GetHeight() > 0) {
		count++;
	}
	if (count >= rowCount) {
		while (this->textEditor->note->GetLength() > 0) {
			this->textEditor->note->Remove(0);
		}
		start = this->textEditor->document->GetStart() - count;
		end = start + rowCount - 1;
		this->textEditor->document->LoadToMemory(start, end, this->textEditor->characterMetrics, &fm);
		this->textEditor->document->SetStart(start);
		this->textEditor->document->SetEnd(end);
	}
	else {
		while (i <= count) {
			this->textEditor->note->Remove(this->textEditor->note->GetLength() - 1);
			i++;
		}
		start = this->textEditor->document->GetStart() - count;
		end = this->textEditor->document->GetStart() - 1;
		this->textEditor->document->LoadToMemory(true, start, end, this->textEditor->characterMetrics, &fm);
		this->textEditor->document->SetStart(start);
		this->textEditor->document->SetEnd(start + rowCount - 1);
	}
	if (this->textEditor->document->isSelecting == true && start <= this->textEditor->document->endPosition.row && end >= this->textEditor->document->startPosition.row) {
		this->textEditor->document->SelectLoadDatas(start, end);
	}
	this->textEditor->scrollController->SetBelow(this->textEditor->document->GetStart() - 1);
	this->textEditor->scrollController->SetVaildMin(this->textEditor->scrollController->GetBelow() * this->textEditor->characterMetrics->GetHeight());
}

void MemoryController::ScrollUp(bool isWrapped, Long position, Long page, Long vaildMin, Long vaildMax) {
	Long i = 1;
	Long start;
	Long end;
	Long distance = vaildMin - position;
	Long count = distance / this->textEditor->characterMetrics->GetHeight();
	Long below = this->textEditor->scrollController->GetBelow();
	Long total;
	Long rowCount = this->textEditor->note->GetRowCount();
	Long line = this->textEditor->document->GetStart();
	FileMaker fm(this->textEditor);

	if (distance % this->textEditor->characterMetrics->GetHeight()) {
		count++;
	}
	this->textEditor->document->CountRows(true, &line, count, this->textEditor->rect.right, this->textEditor->characterMetrics, &total, &fm);
	below -= total;
	count = this->textEditor->document->GetStart() - line;
	if (count >= rowCount) {
		while (this->textEditor->note->GetLength() > 0) {
			this->textEditor->note->Remove(0);
		}
		start = line;
		end = line + rowCount - 1;
		this->textEditor->document->LoadToMemory(start, end, this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
		this->textEditor->document->SetStart(start);
		this->textEditor->document->SetEnd(end);
	}
	else {
		while (i <= count) {
			while (dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(this->textEditor->note->GetLength() - 1))) {
				this->textEditor->note->Remove(this->textEditor->note->GetLength() - 1);
			}
			this->textEditor->note->Remove(this->textEditor->note->GetLength() - 1);
			i++;
		}
		start = line;
		end = start + count - 1;
		this->textEditor->document->LoadToMemory(true, start, end, this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
		this->textEditor->document->SetStart(start);
		this->textEditor->document->SetEnd(start + rowCount - 1);
	}
	if (this->textEditor->document->isSelecting == true &&
		start <= this->textEditor->document->endPosition.row &&
		end >= this->textEditor->document->startPosition.row) {
		this->textEditor->document->SelectLoadDatas(start, end);
	}
	this->textEditor->scrollController->SetBelow(below);
	this->textEditor->scrollController->SetVaildMin(below * this->textEditor->characterMetrics->GetHeight());
	this->textEditor->scrollController->diskFileVSMax = this->textEditor->scrollController->vSMax - (this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight());
}

void MemoryController::ScrollDown(Long position, Long page, Long vaildMin, Long vaildMax){
	Long start;
	Long end;
	Long i = 1;
	Long distance = position - vaildMax;
	Long count = distance / this->textEditor->characterMetrics->GetHeight();
	Long rowCount = this->textEditor->note->GetRowCount();
	FileMaker fm(this->textEditor);

	if (distance % this->textEditor->characterMetrics->GetHeight() > 0) {
		count++;
	}
	if (count >= rowCount) {
		while (this->textEditor->note->GetLength() > 0) {
			this->textEditor->note->Remove(0);
		}
		end = (position + page) / this->textEditor->characterMetrics->GetHeight();
		if ((position + page) / this->textEditor->characterMetrics->GetHeight() > 0) {
			end++;
		}
		start = end - rowCount + 1;
		this->textEditor->document->LoadToMemory(start, end, this->textEditor->characterMetrics, &fm);
		this->textEditor->document->SetStart(start);
		this->textEditor->document->SetEnd(end);
	}
	else {
		while (i <= count) {
			this->textEditor->note->Remove(0);
			i++;
		}
		start = this->textEditor->document->GetStart() + rowCount;
		end = start + count - 1;
		this->textEditor->document->LoadToMemory(start, end, this->textEditor->characterMetrics, &fm);
		this->textEditor->document->SetStart(this->textEditor->document->GetStart() + count);
		this->textEditor->document->SetEnd(end);
	}
	if (this->textEditor->document->isSelecting == true && start <= this->textEditor->document->endPosition.row && end >= this->textEditor->document->startPosition.row) {
		this->textEditor->document->SelectLoadDatas(start, end);
	}
	this->textEditor->scrollController->SetBelow(this->textEditor->document->GetStart() - 1);
	this->textEditor->scrollController->SetVaildMin(this->textEditor->scrollController->GetBelow() * this->textEditor->characterMetrics->GetHeight());
}

void MemoryController::ScrollDown(bool isWrapped, Long position, Long page, Long vaildMin, Long vaildMax) {
	Long i = 1;
	Long start;
	Long end;
	Long distance = position - vaildMax;
	Long count = distance / this->textEditor->characterMetrics->GetHeight();
	Long below = this->textEditor->scrollController->GetBelow();
	Long total;
	Long rowCount = this->textEditor->note->GetRowCount();
	Long line = this->textEditor->document->GetStart() + rowCount - 1;
	FileMaker fm(this->textEditor);

	below += this->textEditor->note->GetLength();
	if (distance % this->textEditor->characterMetrics->GetHeight()) {
		count++;
	}
	this->textEditor->document->CountRows(&line, count,	this->textEditor->rect.right, this->textEditor->characterMetrics, &total, &fm);
	below += total;
	count = line - (this->textEditor->document->GetStart() + rowCount - 1);
	if (count >= rowCount) {
		while (this->textEditor->note->GetLength() > 0) {
			this->textEditor->note->Remove(0);
		}
		start = line - rowCount + 1;
		end = line;
		this->textEditor->document->LoadToMemory(start, end, this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
		this->textEditor->document->SetStart(start);
		this->textEditor->document->SetEnd(end);
	}
	else {
		while (i <= count) {
			this->textEditor->note->Remove(0);
			while (dynamic_cast<DummyRow*>(this->textEditor->note->GetAt(0))) {
				this->textEditor->note->Remove(0);
			}
			i++;
		}
		start = this->textEditor->document->GetStart() + rowCount;
		end = start + count - 1;
		this->textEditor->document->LoadToMemory(start, end, this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
		this->textEditor->document->SetStart(this->textEditor->document->GetStart() + count);
		this->textEditor->document->SetEnd(end);
	}

	if (this->textEditor->document->isSelecting == true && start <= this->textEditor->document->endPosition.row && end >= this->textEditor->document->startPosition.row) {
		this->textEditor->document->SelectLoadDatas(start, end);
	}
	below -= this->textEditor->note->GetLength();
	this->textEditor->scrollController->SetBelow(below);
	this->textEditor->scrollController->SetVaildMin(below * this->textEditor->characterMetrics->GetHeight());
	this->textEditor->scrollController->diskFileVSMax = this->textEditor->scrollController->vSMax - (this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight());
}