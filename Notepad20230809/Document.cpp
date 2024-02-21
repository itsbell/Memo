//Document.cpp
#include "Document.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "CharacterMetrics.h"
#include "DummyRow.h"
#include "Position.h"
#include "SingleByteCharacter.h"
#include "FileMaker.h"
#include "resource.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Document::Document()
: startPosition(-1, -1), endPosition(-1, -1){
	this->paper = 0;
	this->length = 0;
	this->start = 0;
	this->end = 0;
	this->isSelecting = false;
}

Document::~Document() {

}

Long Document::SetStart(Long start) {
	this->start = start;

	return this->start;
}

Long Document::SetEnd(Long end) {
	this->end = end;

	return this->end;
}

Long Document::SetLength(Long length) {
	this->length = length;

	return this->length;
}

void Document::LoadToMemory(Long start, Long end, CharacterMetrics* characterMetrics, FileMaker* fm) {
	char buffer[MAX];
	char character[2];
	Long i = start;
	Long j;
	Long length;
	FILE* file;
	Glyph* row;
	Glyph* glyph;
	GlyphFactory glyphFactory;

	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (i <= end && i <= this->length) {
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &length);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, length, 1, file);
			row = glyphFactory.Create((char*)"\r\n");
			j = 0;
			while (j < length) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					character[0] = buffer[j];
					character[1] = '\0';
				}
				else {
					character[0] = buffer[j];
					character[1] = buffer[j + 1];
					j++;
				}
				glyph = glyphFactory.Create(character);
				row->Add(glyph, characterMetrics);
				j++;
			}
			this->paper->Add(row);
			i++;
		}
		fclose(file);
	}
}

void Document::LoadToMemory(Long start, Long end, CharacterMetrics* characterMetrics, Long width, FileMaker* fm) {
	char buffer[MAX];
	char character[2];
	Long i = start;
	Long j;
	Long length;
	FILE* file;
	Glyph* row;
	Glyph* glyph;
	GlyphFactory glyphFactory;

	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (i <= end && i <= this->length) {
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &length);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, length, 1, file);
			row = glyphFactory.Create((char*)"\r\n");
			j = 0;
			while (j < length) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					character[0] = buffer[j];
					character[1] = '\0';
				}
				else {
					character[0] = buffer[j];
					character[1] = buffer[j + 1];
					j++;
				}
				glyph = glyphFactory.Create(character);
				if (row->GetRowLength() + glyph->GetWidth(characterMetrics) > width) {
					this->paper->Add(row);
					row = glyphFactory.Create((char*)"\r");
				}
				row->Add(glyph, characterMetrics);
				j++;
			}
			this->paper->Add(row);
			i++;
		}
		fclose(file);
	}
}

void Document::LoadToMemory(bool appendFromFront, Long start, Long end, CharacterMetrics* characterMetrics, FileMaker* fm) {
	char buffer[MAX];
	char character[2];
	Long i = start;
	Long j;
	Long index = 0;
	Long length;
	FILE* file;
	Glyph* row;
	Glyph* glyph;
	GlyphFactory glyphFactory;

	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (i <= end && i <= this->length) {
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &length);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, length, 1, file);
			row = glyphFactory.Create((char*)"\r\n");
			j = 0;
			while (j < length) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					character[0] = buffer[j];
					character[1] = '\0';
				}
				else {
					character[0] = buffer[j];
					character[1] = buffer[j + 1];
					j++;
				}
				glyph = glyphFactory.Create(character);
				row->Add(glyph, characterMetrics);
				j++;
			}
			this->paper->Add(index, row);
			index++;
			i++;
		}
		fclose(file);
	}
}

void Document::LoadToMemory(bool appendFromFront, Long start, Long end, CharacterMetrics* characterMetrics, Long width, FileMaker* fm) {
	char buffer[MAX];
	char character[2];
	Long i = start;
	Long j;
	Long index = 0;
	Long length;
	FILE* file;
	Glyph* row;
	Glyph* glyph;
	GlyphFactory glyphFactory;

	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (i <= end && i <= this->length) {
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &length);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, length, 1, file);
			row = glyphFactory.Create((char*)"\r\n");
			j = 0;
			while (j < length) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					character[0] = buffer[j];
					character[1] = '\0';
				}
				else {
					character[0] = buffer[j];
					character[1] = buffer[j + 1];
					j++;
				}
				glyph = glyphFactory.Create(character);
				if (row->GetRowLength() + glyph->GetWidth(characterMetrics) > width) {
					this->paper->Add(index, row);
					index++;
					row = glyphFactory.Create((char*)"\r");
				}
				row->Add(glyph, characterMetrics);
				j++;
			}
			this->paper->Add(index, row);
			index++;
			i++;
		}
		fclose(file);
	}
}

void Document::Load(string pathName, CharacterMetrics* characterMetrics) {
	char character[2];
	char buffer[MAX];
	bool add = true;
	Long i;
	Long j;
	Long index;
	FILE* file;
	Glyph* row;
	Glyph* glyph;
	GlyphFactory glyphFactory;

	//1.파일을 열다.
	file = fopen(pathName.c_str(), "rt");
	//2. 파일이 열리면
	if (file != NULL) {
		//2.1. i가 MAXROW보다 작거나 같고 파일이 끝이 아닌 동안 반복하다.
		i = 0;
		while (i < MAXROW && fgets(buffer, MAX, file) != NULL) {
			//2.1.1. 줄을 만들다.
			glyph = glyphFactory.Create((char*)"\r\n");
			index = this->paper->Add(glyph);
			row = this->paper->GetAt(index);
			//2.1.2. 읽은 줄이 끝이 아니고 개행문자가 아닌 동안 반복하다.
			j = 0;
			while (buffer[j] != '\0' && buffer[j] != '\n' && j < MAX) {
				//2.1.2.1. 읽은 문자가 SingleByte 문자이면 SingleByteCharacter를 생성하여 줄에 추가하다.
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					character[0] = buffer[j];
					character[1] = '\0';
				}
				//2.1.2.2. 읽은 문자가 MultiByte 문자이면 MultiByteCharacter를 생성하여 줄에 추가하다.
				else if (buffer[j] < 0 || buffer[j] >= ASCII) {
					character[0] = buffer[j];
					character[1] = buffer[j + 1];
					j++;
				}
				glyph = glyphFactory.Create(character);
				row->Add(glyph, characterMetrics);
				j++;
			}
			if (buffer[j] != '\n') {
				add = false;
			}
			i++;
		}
		if (add == true && this->paper->GetLength() < MAXROW) {
			//2.2.1. 줄을 만들다.
			glyph = glyphFactory.Create((char*)"\r\n");
			//2.2.2. 줄을 끼우다.
			index = this->paper->Add(glyph);
		}
		//2.3. 적을 위치를 처음으로 이동하다.
		this->paper->First();
		row = this->paper->GetAt(0);
		row->First();

		this->start = 1;
		this->end = this->paper->GetLength();
		fclose(file);
	}
}

void Document::Wrap(Long width, CharacterMetrics* characterMetrics, FileMaker* fm) {
	Long i = 0;
	Glyph* row;
	GlyphFactory glyphFactory;

	if (this->paper != 0) {
		delete this->paper;
	}
	this->paper = glyphFactory.Create((char*)"\0");
	this->LoadToMemory(1, OPTIMAL, characterMetrics, width, fm);
	this->start = 1;
	this->end = this->paper->GetRowCount();
}

void Document::UnWrap(CharacterMetrics* characterMetrics, FileMaker* fm) {
	Long i = 0;
	Glyph* row;
	GlyphFactory glyphFactory;

	if (this->paper != 0) {
		delete this->paper;
	}
	this->paper = glyphFactory.Create((char*)"\0");
	this->LoadToMemory(1, OPTIMAL, characterMetrics, fm);
	this->start = 1;
	this->end = this->paper->GetRowCount();
}

void Document::CountRows(Long* line, Long count, Long rectRight, CharacterMetrics* characterMetrics, Long* total, FileMaker* fm) {
	char buffer[MAX];
	Long i = 1;
	Long j;
	Long width;
	Long length;
	Long characterWidth;
	FILE* file;

	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (i <= count && *line < this->length) {
			(*line)++;
			fseek(file, (*line - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &length);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, length, 1, file);
			width = rectRight;
			j = 0;
			while (j < length) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					characterWidth = characterMetrics->GetWidth(buffer[j]);
				}
				else {
					characterWidth = characterMetrics->GetWidth(128);
					j++;
				}
				width -= characterWidth;
				if (width < 0) {
					i++;
					width = rectRight;
					width -= characterWidth;
				}
				j++;
			}
			i++;
		}
		fclose(file);
	}
	*total = i - 1;
}

void Document::CountRows(bool reverse, Long* line, Long count, Long rectRight, CharacterMetrics* characterMetrics, Long* total, FileMaker* fm) {
	char buffer[MAX];
	Long i = 1;
	Long j;
	Long width;
	Long length;
	Long characterWidth;
	FILE* file;

	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (i <= count && *line > 1) {
			(*line)--;
			fseek(file, (*line - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &length);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, length, 1, file);
			width = rectRight;
			j = 0;
			while (j < length) {
				if (buffer[j] >= 0 && buffer[j] < ASCII) {
					characterWidth = characterMetrics->GetWidth(buffer[j]);
				}
				else {
					characterWidth = characterMetrics->GetWidth(128);
					j++;
				}
				width -= characterWidth;
				if (width < 0) {
					i++;
					width = rectRight;
					width -= characterWidth;
				}
				j++;
			}
			i++;
		}
		fclose(file);
	}
	*total = i - 1;
}

void Document::CountRows(Long start, Long number, Long below, Long rectRight, CharacterMetrics* characterMetrics, Long* row, Long* count, FileMaker* fm){
	char buffer[MAX];
	Long i = start;
	Long j = below;
	Long k;
	Long line;
	Long byte;
	Long charWidth;
	Long left;
	FILE* file;

	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (i <= this->length && j < number) {
			fseek(file, (i - 1) * (MAX + 12), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			left = rectRight;
			j++;
			line = j;
			k = 0;
			while (k < byte) {
				if (buffer[k] >= 0 && buffer[k] < ASCII) {
					charWidth = characterMetrics->GetWidth(buffer[k]);
				}
				else {
					charWidth = characterMetrics->GetWidth(128);
					k++;
				}
				left -= charWidth;
				if (left < 0) {
					j++;
					left = rectRight - charWidth;
				}
				k++;
			}
			i++;
		}
		fclose(file);
	}
	*row = i - 1;
	*count = number - line;
}

Long Document::GetRowNumber() {
	Long i = 0;
	Long count = 0;
	Glyph* row;

	while (i < this->paper->GetCurrent()) {
		row = this->paper->GetAt(i);
		if (!dynamic_cast<DummyRow*>(row)) {
			count++;
		}
		i++;
	}
	count += this->start - 1;

	return count;
}

/*
함수 명칭 : GetRowNumber
기     능 : 스크롤링 전 위치를 현재 메모리내에서 몇 번째 줄인지 찾다.
입     력 : 줄 번호
출     력 : 줄 번호
*/
Long Document::GetRowNumber(bool rowOrDummy, Long row) {
	Long i = 0;
	Long j = 0;
	Long index;
	Glyph* glyph;
	
	if (rowOrDummy) {
		index = this->start - 1;
		while (i < this->paper->GetLength() && index < row) {
			glyph = this->paper->GetAt(i);
			if (!(dynamic_cast<DummyRow*>(glyph))) {
				index++;
			}
			i++;
		}
		if (i == 0) {
			i++;
		}
	}
	// 자동줄바꿈 상태에서 줄번호를 입력하면 문서에서 몇번째 row인지 알 수 있다.
	else {
		while (j < row) {
			glyph = this->paper->GetAt(j);
			if (!(dynamic_cast<DummyRow*>(glyph))) {
				i++;
			}
			j++;
		}
		i += (this->start - 1);
	}

	return i;
}

Position Document::GetPosition() {
	Long i = 0;
	Long row;
	Long below = 0;
	Long currentRow = this->paper->GetCurrent();
	Long column;
	Glyph* glyph;
	Glyph* current;

	if (this->length > 0) {
		below = this->start - 1;
	}
	row = below;
	while (i < currentRow) {
		glyph = this->paper->GetAt(i);
		if (!(dynamic_cast<DummyRow*>(glyph))) {
			row++;
		}
		i++;
	}
	i = this->paper->GetCurrent();
	current = this->paper->GetAt(currentRow - 1);
	column = current->GetCurrent();
	while (dynamic_cast<DummyRow*>(current)) {
		i--;
		column += this->paper->GetAt(i - 1)->GetLength();
		current = this->paper->GetAt(i - 1);
	}

	return Position(row, column);
}

void Document::Select(Position previous, Position current) {
	bool going = true;
	Long i = 0;
	Long j;
	Long k;
	Long l = 1;
	Long count;
	Long second = 0;
	Long third = 0;
	Long forth = 0;
	Glyph* row;
	Glyph* glyph;

	if (previous.row < current.row) {
		if (previous.row >= this->start) {
			i = this->GetRowNumber(true, previous.row);
			j = previous.column;
			second = this->GetRowNumber(true, previous.row + 1);
		}
		else {
			i = 0;
			second = 1;
		}
		third = this->GetRowNumber(true, current.row);
		forth = third;
		k = current.column;
	}
	else if (previous.row > current.row) {
		i = this->GetRowNumber(true, current.row);
		j = current.column;
		second = this->GetRowNumber(true, current.row + 1);
		if (previous.row <= this->start + this->paper->GetRowCount()) {
			third = this->GetRowNumber(true, previous.row);
			forth = third;
			k = previous.column;
		}
		else {
			third = this->paper->GetLength() + 1;
		}
	}
	else if (previous.row == current.row && previous.column < current.column) {
		i = this->paper->GetCloseRow(this->paper->GetCurrent());
		j = previous.column;
		k = current.column;
		count = k - j;
		going = false;
	}
	else if (previous.row == current.row && previous.column > current.column) {
		i = this->paper->GetCloseRow(this->paper->GetCurrent());
		j = current.column;
		k = previous.column;
		count = k - j;
		going = false;
	}

	if (i > 0) {
		row = this->paper->GetAt(i - 1);
		while (j >= row->GetLength() && i < this->paper->GetLength() && dynamic_cast<DummyRow*>(this->paper->GetAt(i))) {
			j -= row->GetLength();
			row = this->paper->GetAt(i);
			i++;
		}
		if (going == true) {
			row->SelectContents(j, row->GetLength());
			while (i < this->paper->GetLength() && dynamic_cast<DummyRow*>(this->paper->GetAt(i))) {
				row = this->paper->GetAt(i);
				row->SelectContents();
				i++;
			}
		}
		else {
			while (l <= count) {
				if (j == row->GetLength()) {
					row = this->paper->GetAt(i);
					i++;
					j = 0;
				}
				glyph = row->GetAt(j);
				if (glyph->GetIsSelected() == false) {
					glyph->Select();
				}
				else {
					glyph->UnSelect();
				}
				j++;
				l++;
			}
		}
	}
	i = second;
	while (i < third) {
		row = this->paper->GetAt(i - 1);
		if (!(dynamic_cast<DummyRow*>(row))) {
			if (row->GetIsSelected() == false) {
				row->Select();
			}
			else {
				row->UnSelect();
			}
		}
		row->SelectContents();
		i++;
	}
	i = forth;
	if (i > 0) {
		row = this->paper->GetAt(i - 1);
		if (row->GetIsSelected() == false) {
			row->Select();
		}
		else {
			row->UnSelect();
		}
		while (row->GetLength() < k) {
			row->SelectContents();
			k -= row->GetLength();
			i++;
			row = this->paper->GetAt(i - 1);
		}
		row->SelectContents(0, k);
	}
}

void Document::OnlySelect(Position previous, Position current) {
	Long i = 0;
	Long j;
	Long k;
	Long l = 1;
	Long count;
	Glyph* row;
	Glyph* glyph;

	if (previous.row == current.row && previous.column < current.column) {
		i = this->GetRowNumber(true, previous.row);
		j = previous.column;
		k = current.column;
		count = k - j;
	}
	else if (previous.row == current.row && previous.column > current.column) {
		i = this->GetRowNumber(true, current.row);
		j = current.column;
		k = previous.column;
		count = k - j;
	}
	if (i > 0) {
		row = this->paper->GetAt(i - 1);
		while (j >= row->GetLength() && i < this->paper->GetLength() && dynamic_cast<DummyRow*>(this->paper->GetAt(i))) {
			j -= row->GetLength();
			row = this->paper->GetAt(i);
			i++;
		}
		while (l <= count) {
			if (j == row->GetLength()) {
				row = this->paper->GetAt(i);
				i++;
				j = 0;
			}
			glyph = row->GetAt(j);
			glyph->Select();
			j++;
			l++;
		}
	}
}

void Document::SetSelectingInfo(Position previous, Position current) {
	Long i = this->paper->GetCurrent();
	Glyph* row = this->paper->GetAt(i - 1);
	Long j = row->GetCurrent();
	Glyph* glyph;

	if (j < row->GetLength()) {
		glyph = row->GetAt(j);
		if (glyph->GetIsSelected()) {
			this->startPosition = current;
			this->endPosition = previous;
			this->isSelecting = true;
		}
		else {
			if (j > 0) {
				j--;
				glyph = row->GetAt(j);
			}
			else if (j == 0 && !(dynamic_cast<DummyRow*>(row))) {
				glyph = row;
			}
			else if (j == 0 && dynamic_cast<DummyRow*>(row)){
				i--;
				row = this->paper->GetAt(i - 1);
				glyph = row->GetAt(row->GetLength() - 1);
			}
			if (glyph->GetIsSelected() == true) {
				this->startPosition = previous;
				this->endPosition = current;
				this->isSelecting = true;
			}
		}
	}
	else if (j == row->GetLength() && row->GetLength() > 0) {
		j--;
		glyph = row->GetAt(j);
		if (glyph->GetIsSelected() == true) {
			this->startPosition = previous;
			this->endPosition = current;
			this->isSelecting = true;
		}
		else {
			if (i < this->paper->GetLength()) {
				i++;
				row = this->paper->GetAt(i - 1);
				glyph = row;
				if (dynamic_cast<DummyRow*>(row)) {
					glyph = row->GetAt(0);
				}
				if (glyph->GetIsSelected() == true) {
					this->startPosition = current;
					this->endPosition = previous;
					this->isSelecting = true;
				}
			}
		}
	}
	else if (j == row->GetLength() && row->GetLength() == 0) {
		if (row->GetIsSelected() == true) {
			this->startPosition = previous;
			this->endPosition = current;
			this->isSelecting = true;
		}
		else {
			if (i < this->paper->GetLength()) {
				i++;
				row = this->paper->GetAt(i - 1);
				if (row->GetIsSelected() == true) {
					this->startPosition = current;
					this->endPosition = previous;
					this->isSelecting = true;
				}
			}
		}
	}
}

void Document::SetSelectingInfo(bool isSelecting, Position previous, Position current) {
	Long i = this->paper->GetCurrent();
	Glyph* row = this->paper->GetAt(i - 1);
	Long j = row->GetCurrent();
	Glyph* glyph;
	Long rowNumber;

	if (j < row->GetLength()) {
		glyph = row->GetAt(j);
		if (glyph->GetIsSelected()) {
			if (previous > this->startPosition && current < this->startPosition) {
				this->endPosition = this->startPosition;
			}
			this->startPosition = current;
		}
		else {
			if (j > 0) {
				j--;
				glyph = row->GetAt(j);
			}
			else if (j == 0 && !(dynamic_cast<DummyRow*>(row))) {
				glyph = row;
			}
			else if (j == 0 && dynamic_cast<DummyRow*>(row)) {
				i--;
				row = this->paper->GetAt(i - 1);
				glyph = row->GetAt(row->GetLength() - 1);
			}
			if (glyph->GetIsSelected()) {
				if (previous < this->endPosition && current > this->startPosition) {
					this->startPosition = this->endPosition;
				}
				this->endPosition = current;
			}
			else {
				this->isSelecting = false;
			}
		}
	}
	else if (j == row->GetLength() && row->GetLength() > 0) {
		glyph = row->GetAt(j - 1);
		if (glyph->GetIsSelected()) {
			if (previous < this->endPosition && current > this->startPosition) {
				this->startPosition = this->endPosition;
			}
			this->endPosition = current;
		}
		else {
			if (i < this->paper->GetLength()) {
				i++;
				row = this->paper->GetAt(i - 1);
				glyph = row;
				if (dynamic_cast<DummyRow*>(row)) {
					glyph = row->GetAt(0);
				}
				if (glyph->GetIsSelected()) {
					if (previous > this->startPosition && current < this->startPosition) {
						this->endPosition = this->startPosition;
					}
					this->startPosition = current;
				}
				else {
					this->isSelecting = false;
				}
			}
			else {
				if (this->end < this->length) {
					rowNumber = this->GetRowNumber(false, i) + 1;
					if (rowNumber <= this->endPosition.row) {
						this->startPosition = current;
					}
					else {
						this->isSelecting = false;
					}
				}
				else {
					this->isSelecting = false;
				}
			}
		}
	}
	else if (j == row->GetLength() && row->GetLength() == 0) {
		if (row->GetIsSelected() == true) {
			if (previous <this->endPosition && current > this->startPosition) {
				this->startPosition = this->endPosition;
			}
			this->endPosition = current;
		}
		else {
			if (i < this->paper->GetLength()) {
				i++;
				row = this->paper->GetAt(i - 1);
				if (row->GetIsSelected() == true) {
					this->startPosition = current;
				}
				else {
					this->isSelecting = false;
				}
			}
			else {
				if (this->end < this->length) {
					rowNumber = this->GetRowNumber(false, i) + 1;
					if (rowNumber <= this->endPosition.row) {
						this->startPosition = current;
					}
					else {
						this->isSelecting = false;
					}
				}
				else {
					this->isSelecting = false;
				}
			}
		}
	}
}

void Document::UpdateSelectingInfo(Position previous, Position current) {
	if (this->isSelecting == false) {
		this->SetSelectingInfo(previous, current);
	}
	else {
		this->SetSelectingInfo(true, previous, current);
	}
}

void Document::SelectAll()
{
	Long i = 0;
	Long j;
	Long column;
	Glyph* row;
	Glyph* character;

	while (i < this->paper->GetLength()) {
		row = this->paper->GetAt(i);
		j = 0;
		while (j < row->GetLength()) {
			character = row->GetAt(j);
			character->Select();
			j++;
		}
		if (!(this->start == 1 && i == 0) && !dynamic_cast<DummyRow*>(row)) {
			row->Select();
		}
		i++;
	}
	this->paper->Last();
	row = this->paper->GetAt(this->paper->GetCurrent() - 1);
	row->Last();
	column = this->paper->GetColumnNumber();
	this->startPosition.SetPosition(1, 0);
	this->endPosition.SetPosition(this->length, column);
}

void Document::SelectLoadDatas(Long start, Long end) {
	Long startRow = start;
	Long endRow = end;
	Long i;
	Long indexes[2];
	Glyph* row;
	
	while (startRow < this->startPosition.row) {
		startRow++;
	}
	indexes[0] = this->GetRowNumber(true, startRow);
	if (end > this->endPosition.row) {
		endRow = this->endPosition.row;
	}
	indexes[1] = this->GetRowNumber(true, endRow);
	
	row = this->paper->GetAt(indexes[0] - 1);
	if (this->startPosition.row == this->endPosition.row) {
		this->OnlySelect(this->startPosition, this->endPosition);
	}
	else {
		if (startRow == this->startPosition.row) {
			i = this->startPosition.column;
			while (i > row->GetLength()) {
				i -= row->GetLength();
				indexes[0]++;
				row = this->paper->GetAt(indexes[0] - 1);
			}
			row->SelectContents(i, row->GetLength());
			indexes[0]++;
			while (indexes[0] <= this->paper->GetLength() && dynamic_cast<DummyRow*>(this->paper->GetAt(indexes[0] - 1))) {
				row = this->paper->GetAt(indexes[0] - 1);
				row->SelectContents();
				indexes[0]++;
			}
		}
		
		if (endRow < this->endPosition.row) {
			while (indexes[0] <= indexes[1]) {
				row = this->paper->GetAt(indexes[0] - 1);
				if (!dynamic_cast<DummyRow*>(row)) {
					row->Select();
				}
				row->SelectContents();
				indexes[0]++;
				while (indexes[0] <= this->paper->GetLength() && dynamic_cast<DummyRow*>(this->paper->GetAt(indexes[0] - 1))) {
					row = this->paper->GetAt(indexes[0] - 1);
					row->SelectContents();
					indexes[0]++;
				}
				//DummyRow이면 끝까지 선택해야함.
			}
		}
		else{
			while (indexes[0] < indexes[1]) {
				row = this->paper->GetAt(indexes[0] - 1);
				if (!dynamic_cast<DummyRow*>(row)) {
					row->Select();
				}
				row->SelectContents();
				indexes[0]++;
				while (dynamic_cast<DummyRow*>(this->paper->GetAt(indexes[0] - 1))) {
					row = this->paper->GetAt(indexes[0] - 1);
					row->SelectContents();
					indexes[0]++;
				}
			}
			row = this->paper->GetAt(indexes[1] - 1);
			row->Select();
			i = this->endPosition.column;
			while (i > row->GetLength()) {
				row->SelectContents();
				i -= row->GetLength();
				indexes[0]++;
				row = this->paper->GetAt(indexes[0] - 1);
			}
			row->SelectContents(0, i);
		}
	}
}

void Document::Move(Long row, Long column) {
	Long i = 0;
	Long count = 0;
	Glyph* glyph;

	row -= (this->start - 1);
	while (count < row) {
		glyph = this->paper->GetAt(i);
		i++;
		if (!(dynamic_cast<DummyRow*>(glyph))) {
			count++;
		}
	}
	this->paper->Move(i);
	glyph = this->paper->GetAt(i - 1);
	while (column > glyph->GetLength()) {
		column -= glyph->GetLength();
		i = this->paper->Next();
		glyph = this->paper->GetAt(i - 1);
	}
	glyph->Move(column);
}

void Document::Move(Long row, Long column, char* logPath) {
	Long i = 0;
	Long count = 0;
	Glyph* glyph;
	FILE* file;

	file = fopen(logPath, "at");
	if (file != NULL) {
		glyph = this->paper->GetAt(i);
		fprintf(file, "Document::Move / first Iteration before start / document.start: %d, document.end: %d, document.length: %d, note.current: %d, note.rowCont: %d, note.length: %d\n", this->start, this->end, this->length, this->paper->GetCurrent(), this->paper->GetRowCount(), this->paper->GetLength());
		row -= (this->start - 1);
		while (count < row) {
			glyph = this->paper->GetAt(i);
			i++;
			if (!(dynamic_cast<DummyRow*>(glyph))) {
				count++;
			}
		}
		this->paper->Move(i);
		glyph = this->paper->GetAt(i - 1);
		fprintf(file, "Document::Move / second Iteration before start / document.start: %d, document.end: %d, document.length: %d, note.current: %d, note.rowCont: %d, note.length: %d\n", this->start, this->end, this->length, this->paper->GetCurrent(), this->paper->GetRowCount(), this->paper->GetLength());
		while (column > glyph->GetLength()) {
			column -= glyph->GetLength();
			i = this->paper->Next();
			glyph = this->paper->GetAt(i - 1);
		}
		glyph->Move(column);

		fclose(file);
	}
}

void Document::UnSelect() {
	Long i = 0;
	Long j = this->startPosition.column;
	Long rowNumber = this->start - 1;
	Glyph* row = 0;
	Glyph* character;

	if (this->start <= this->endPosition.row && this->end >= this->startPosition.row) {
		if (this->start <= this->startPosition.row) {
			while (rowNumber < this->startPosition.row) {
				row = this->paper->GetAt(i);
				i++;
				if (!(dynamic_cast<DummyRow*>(row))) {
					rowNumber++;
				}
			}
			while (j > row->GetLength()) {
				j -= row->GetLength();
				i++;
				row = this->paper->GetAt(i - 1);
			}
			while (this->isSelecting == true && j < row->GetLength()) {
				character = row->GetAt(j);
				if (character->GetIsSelected() == true) {
					character->UnSelect();
				}
				else {
					this->isSelecting = false;
				}
				j++;
			}
			i++;
		}
		else {
			i = 1;
		}
		while (this->isSelecting == true && i <= this->paper->GetLength()) {
			row = this->paper->GetAt(i - 1);
			if (!(dynamic_cast<DummyRow*>(row))) {
				if (row->GetIsSelected() == true) {
					row->UnSelect();
				}
				else {
					this->isSelecting = false;
				}
			}
			j = 0;
			while (this->isSelecting == true && j < row->GetLength()) {
				character = row->GetAt(j);
				if (character->GetIsSelected() == true) {
					character->UnSelect();
				}
				else {
					this->isSelecting = false;
				}
				j++;
			}
			i++;
		}
	}
	else {
		this->isSelecting = false;
	}
}

string Document::Copy(Long row) {
	Long index = this->GetRowNumber(true, row);
	string contents;
	Glyph* glyph;

	glyph = this->paper->GetAt(index - 1);
	contents = glyph->Copy();
	index++;
	while (index <= this->paper->GetLength() && dynamic_cast<DummyRow*>(this->paper->GetAt(index - 1))) {
		glyph = this->paper->GetAt(index - 1);
		contents += glyph->Copy();
		index++;
	}
	return contents;
}

void Document::Copy(Long row, FILE* file, char* contents, Long* length) {
	bool startOrEnd = false;
	Long i = 0;
	Long j = 0;
	Long byte;
	Long point = (row - 1) * (12 + MAX);
	
	fseek(file, point, SEEK_SET);
	fscanf(file, "%d", &byte);
	fseek(file, 8, SEEK_CUR);
	fread(contents, byte, 1, file);
	*length = byte;

	if (row == this->startPosition.row) {
		while (i < this->startPosition.column) {
			if (contents[j] < 0 || contents[j] >= ASCII) {
				j++;
			}
			j++;
			i++;
		}
		point = (row - 1) * (12 + MAX) + 12 + j;
		*length -= j;
		startOrEnd = true;
	}
	if (row == this->endPosition.row) {
		j = 0;
		i = 0;
		while (i < this->endPosition.column) {
			if (contents[j] < 0 || contents[j] >= ASCII) {
				j++;
			}
			j++;
			i++;
		}
		*length -= (byte - j);
		if (startOrEnd == false) {
			point = (row - 1) * (12 + MAX) + 12;
		}
		startOrEnd = true;
	}
	if (startOrEnd == true) {
		fseek(file, point, SEEK_SET);
		memset(contents, 0, MAX);
		fread(contents, *length, 1, file);
	}
}

void Document::PasteFromFile(string fileName, Long start, Long end, CharacterMetrics* characterMetrics, bool isWrapped, Long width) {
	bool going = true;
	char character[2];
	char buffer[MAX];
	Long i;
	Long j = 0;
	Long k;
	Long count;
	Long point;
	FILE* file;
	Glyph* row;
	Glyph* glyph;
	Glyph* current;
	GlyphFactory glyphFactory;

	current = glyphFactory.Create((char*)"\r\n");
	if (isWrapped) {
		this->paper->RowUnWrap(characterMetrics);
	}
	i = this->paper->GetCurrent();
	row = this->paper->GetAt(i - 1);
	while (j < row->GetCurrent()) {
		glyph = row->GetAt(j);
		current->Add(glyph->Clone(), characterMetrics);
		j++;
	}
	file = fopen(fileName.c_str(), "rt");
	if (file != NULL) {
		fseek(file, start, SEEK_SET);
		point = ftell(file);
		while (point < end) {
			fgets(buffer, MAX, file);
			if (ftell(file) == end) {
				going = false;
			}
			k = 0;
			while (buffer[k] != '\0') {
				character[0] = buffer[k];
				if (character[0] == '\r') {
					k++;
					if (going == true) {
						character[1] = buffer[k];
						if (this->paper->GetCurrent() < this->paper->GetLength()) {
							this->paper->Add(this->paper->GetCurrent(), current);
						}
						else {
							this->paper->Add(current);
						}
						current = glyphFactory.Create(character);
					}
				}
				else if (buffer[k] >= 0 && buffer[k] < ASCII) {
					character[1] = '\0';
					glyph = glyphFactory.Create(character);
					current->Add(glyph, characterMetrics);
				}
				else {
					k++;
					character[1] = buffer[k];
					glyph = glyphFactory.Create(character);
					current->Add(glyph, characterMetrics);
				}
				k++;
			}
			point = ftell(file);
		}
		if (this->paper->GetCurrent() < this->paper->GetLength()) {
			this->paper->Add(this->paper->GetCurrent(), current);
		}
		else {
			this->paper->Add(current);
		}
		fclose(file);
	}
	k = current->GetCurrent();
	while (j < row->GetLength()) {
		glyph = row->GetAt(j);
		current->Add(glyph->Clone(), characterMetrics);
		j++;
	}
	current->Move(k);
	k = this->paper->GetCurrent();
	this->paper->Remove(i - 1);
	k--;
	this->paper->Move(k);
	
	if (isWrapped) {
		count = k - i;
		j = 1;
		while (j <= count) {
			this->paper->WrapRow(i - 1, width, characterMetrics, false);
			i++;
			while (dynamic_cast<DummyRow*>(this->paper->GetAt(i - 1))) {
				i++;
			}
			j++;
		}
		this->paper->WrapRow(i - 1, width, characterMetrics, true);
	}
#if 0
		fgets(buffer, MAX, file);
		k = 0;
		while (buffer[k] != '\0') {
			character[0] = buffer[k];
			if (buffer[k] >= 0 && buffer[k] < ASCII) {
				character[1] = '\0';
			}
			else {
				k++;
				character[1] = buffer[k];
			}
			glyph = glyphFactory.Create(character);
			current->Add(glyph, characterMetrics);
			k++;
		}
		if (this->paper->GetCurrent() < this->paper->GetLength()) {
			this->paper->Add(this->paper->GetCurrent(), current);
		}
		else {
			this->paper->Add(current);
		}
		point = ftell(file);
		while (point < end) {
			current = glyphFactory.Create((char*)"\r\n");
			fgets(buffer, MAX, file);
			k = 0;
			while (buffer[k] != '\r' && buffer[k] != '\0') {
				character[0] = buffer[k];
				if (buffer[k] >= 0 && buffer[k] < ASCII) {
					character[1] = '\0';
				}
				else {
					k++;
					character[1] = buffer[k];
				}
				glyph = glyphFactory.Create(character);
				current->Add(glyph, characterMetrics);
				k++;
			}
			if (this->paper->GetCurrent() < this->paper->GetLength()) {
				this->paper->Add(this->paper->GetCurrent(), current);
			}
			else {
				this->paper->Add(current);
			}
			point = ftell(file);
		}
		fclose(file);
	}
	k = j;
	while (j < row->GetLength()) {
		glyph = row->GetAt(j);
		current->Add(glyph->Clone(), characterMetrics);
		j++;
	}
	current->Move(k);
	k = this->paper->GetCurrent();
	this->paper->Remove(i - 1);
	k--;
	if (isWrapped) {
		while (i < k) {
			this->paper->WrapRow(this->GetRowNumber(true, i), width, characterMetrics, false);
			i++;
		}
		this->paper->WrapRow(this->GetRowNumber(true, i), width, characterMetrics, true);
	}
#endif
}

Long Document::GetIndex(char* text, Long column) {
	Long i = 0;
	Long j = 0;
	
	while (i < column) {
		if (text[j] < 0 || text[j] >= ASCII) {
			j++;
		}
		j++;
		i++;
	}
	return j;
}

void Document::AddDeletedContents(Position start, Position end, Long* startPointer, Long* endPointer, FileMaker* fm) {
	char text[MAX];
	char contents[MAX];
	Long i = start.row;
	Long j;
	Long k;
	Long l;
	Long byte;
	Long count;
	Long length;
	Long number;
	Long rowNumber;
	FILE* file;
	FILE* del;
	Glyph* row;

	file = fopen(fm->GetFile(), "rt");
	del = fopen(fm->GetDel(), "at");
	if (file != NULL && del != NULL) {
		fseek(del, 0, SEEK_END);
		*startPointer = ftell(del);
		while (i <= end.row && i < this->start) {
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(contents, byte, 1, file);
			if (i > start.row && i < end.row) {
				fwrite(contents, byte, 1, del);
				fprintf(del, "\r\n", contents);
			}
			else if (i == start.row && i == end.row) {
				k = 0;
				byte = this->GetIndex(contents, end.column);
				j = this->GetIndex(contents, start.column);
				while (j < byte) {
					text[k] = contents[j];
					k++;
					j++;
				}
				if (k > 0) {
					byte = k;
					fwrite(text, byte, 1, del);
				}
				fprintf(del, "\r\n");
				*endPointer = ftell(del);
			}
			else if (i == start.row) {
				k = 0;
				j = this->GetIndex(contents, start.column);
				while (j < byte) {
					text[k] = contents[j];
					k++;
					j++;
				}
				if (k >= 0) {
					byte = k;
					fwrite(text, byte, 1, del);
					fprintf(del, "\r\n");
				}
			}
			else if (i == end.row) {
				byte = this->GetIndex(contents, end.column);
				fwrite(contents, byte, 1, del);
				fprintf(del, "\r\n");
				*endPointer = ftell(del);
			}
			i++;
		}
		while (i <= end.row && i <= this->start + this->paper->GetRowCount() - 1) {
			rowNumber = this->GetRowNumber(true, i);
			row = this->paper->GetAt(rowNumber - 1);
			j = 0;
			k = row->GetLength();
			number = rowNumber;
			while (number < this->paper->GetLength() && dynamic_cast<DummyRow*>(this->paper->GetAt(number))) {
				number++;
				k += this->paper->GetAt(number - 1)->GetLength();
			}
			if (i == start.row && i == end.row) {
				j = start.column;
				k = end.column;
			}
			else if (i == start.row) {
				j = start.column;
			}
			else if (i == end.row) {
				k = end.column;
			}
			count = k - j;
			while (j > row->GetLength()) {
				rowNumber++;
				j -= row->GetLength();
				row = this->paper->GetAt(rowNumber - 1);
			}
			l = 1;
			k = 0;
			byte = 0;
			while (l <= count) {
				row->GetContents(j, count, &l, &k, contents);
				byte = k;
				if (l <= count) {
					j = 0;
					rowNumber++;
					row = this->paper->GetAt(rowNumber - 1);
				}
			}
			fwrite(contents, byte, 1, del);
			fprintf(del, "\r\n");
			if (i == end.row) {
				*endPointer = ftell(del);
			}
			i++;
		}

		length = this->start + this->paper->GetLength() - 1 + (this->length - this->end);
		while (i <= end.row && i <= length) {
			fseek(file, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(contents, byte, 1, file);
			if (i > start.row && i < end.row) {
				fwrite(contents, byte, 1, del);
				fprintf(del, "\r\n", contents);
			}
			else if (i == start.row && i == end.row) {
				k = 0;
				byte = this->GetIndex(contents, end.column);
				j = this->GetIndex(contents, start.column);
				while (j < byte) {
					text[k] = contents[j];
					k++;
					j++;
				}
				if (k > 0) {
					byte = k;
					fwrite(text, byte, 1, del);
				}
				fprintf(del, "\r\n");
				*endPointer = ftell(del);
			}
			else if (i == start.row) {
				k = 0;
				j = this->GetIndex(contents, start.column);
				while (j < byte) {
					text[k] = contents[j];
					k++;
					j++;
				}
				if (k > 0) {
					byte = k;
					fwrite(text, byte, 1, del);
				}
				fprintf(del, "\r\n");
			}
			else if (i == end.row) {
				byte = this->GetIndex(contents, end.column);
				fwrite(contents, byte, 1, del);
				fprintf(del, "\r\n");
				*endPointer = ftell(del);
			}
			i++;
		}
		fclose(file);
		fclose(del);
	}
}

void Document::Delete(bool isWrapped, Long width, CharacterMetrics* characterMetrics, FileMaker* fm) {
	char buffer[MAX];
	char contents[MAX - 2];
	FILE* file;
	FILE* temp;
	Long i = 1;
	Long byte;
	Long index;
	Long count;
	Long rowLength;
	Position end;
	Glyph* row;
	
	file = fopen(fm->GetFile(), "rt");
	temp = fopen(fm->GetTemp(), "wt+");
	if (file != NULL && temp != NULL) {
		while (i < this->start) {
			if (i < this->startPosition.row || i > this->endPosition.row) {
				fseek(file, ((i - 1) * (12 + MAX)), SEEK_SET);
				fgets(buffer, MAX, file);
				fputs(buffer, temp);
				fgets(buffer, MAX, file);
				fputs(buffer, temp);
			}
			i++;
		}
		count = i - 1;
		this->paper->Last();
		row = this->paper->GetAt(this->paper->GetCurrent() - 1);
		row->Last();
		end = this->GetPosition();
		this->paper->Delete(this->start, this->startPosition, end, isWrapped, characterMetrics, width);
		i = 0;
		while (i < this->paper->GetLength()) {
			this->paper->ConvertChar(&i, &byte, &rowLength, contents);
			fprintf(temp, "%04d %05d\n", byte, rowLength);
			fwrite(contents, MAX - 2, 1, temp);
			count++;
			if (i < this->paper->GetLength()) {
				fputs("\n", temp);
			}
		}
		i = this->end + 1;
		bool newLine = true;
		while (i <= this->length) {
			if (i < this->startPosition.row || i > this->endPosition.row) {
				if (newLine) {
					puts("\n");
					newLine = false;
				}
				fseek(file, ((i - 1) * (12 + MAX)), SEEK_SET);
				fgets(buffer, MAX, file);
				fputs(buffer, temp);
				fgets(buffer, MAX, file);
				fputs(buffer, temp);
				count++;
			}		
			else if (i == this->endPosition.row) {
				fseek(file, ((i - 1) * (12 + MAX)), SEEK_SET);
				fscanf(file, "%d", &byte);
				fseek(file, 8, SEEK_CUR);
				fread(buffer, byte, 1, file);
				index = this->GetIndex(buffer, this->endPosition.column);
				fm->ConvertChar(buffer, &byte, index, false, &rowLength);

				index = this->paper->Last();
				row = this->paper->GetAt(index - 1);
				char character[2];
				Glyph* glyph;
				GlyphFactory glyphFactory;
				index = 0;
				while (index < byte) {
					if (buffer[index] >= 0 && buffer[index] < 127) {
						character[0] = buffer[index];
						character[1] = '\0';
					}
					else {
						character[0] = buffer[index];
						character[1] = buffer[index + 1];
						index++;
					}
					glyph = glyphFactory.Create(character);
					row->Add(glyph, characterMetrics);
					index++;
				}
				Long beforeByte;
				Long beforeRowLength;
				char contents[MAX];
				memset(contents, 127, MAX);
				fseek(temp, (count - 1) * (12 + MAX), SEEK_SET);
				fscanf(temp, "%d", &beforeByte);
				fseek(temp, 1, SEEK_CUR);
				fscanf(temp, "%d", &beforeRowLength);
				fseek(temp, 2, SEEK_CUR);
				fread(contents, beforeByte, 1, temp);
				index = 0;
				while (index < byte) {
					contents[beforeByte] = buffer[index];
					beforeByte++;
					index++;
				}
				rowLength += beforeRowLength;
				fseek(temp, (count - 1) * (12 + MAX), SEEK_SET);
				fprintf(temp, "%04d %05d\n", beforeByte, rowLength);
				fwrite(contents, MAX - 2, 1, temp);
				if (i < this->length) {
					fputs("\n", temp);
				}
				newLine = false;
			}
			i++;
		}
		fclose(file);
		fclose(temp);
	}
	remove(fm->GetFile());
	rename(fm->GetTemp(), fm->GetFile());
	this->length = count;
	count = this->endPosition.row - this->startPosition.row;
	if (this->endPosition.row < this->start) {
		this->start -= count;
		this->end -= count;
	}
	else if (this->startPosition.row < this->start && this->endPosition.row <= this->end) {
		this->start = this->startPosition.row;
		this->end -= count;
	}
	else if (this->startPosition.row >= this->start && this->endPosition.row <= this->end) {
		this->end -= count;
	}
	else if (this->startPosition.row >= this->start && this->endPosition.row > this->end) {
		this->end = this->startPosition.row;
	}
	else if (this->startPosition.row <= this->end) {
		this->end = this->startPosition.row;
	}
}

void Document::FindNext(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm) {
	char buffer[MAX];
	Long i;
	Long j;
	Long k;
	Long byte;
	Long line = startRow;
	Long column;
	FILE* file;

	*found = false;
	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (line <= this->length && *found == false) {
			fseek(file, (line - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			i = 0;
			if (line != startRow) {
				j = 0;
			}
			else {
				j = this->GetIndex(buffer, startColumn);
			}
			while (i < byte && findWhat[j] != '\0') {
				if (j == 0) {
					*start = i;
				}
				if (buffer[i] >= 65 && buffer[i] <= 90) {
					if (buffer[i] == findWhat[j] || buffer[i] + 32 == findWhat[j]) {
						i++;
						j++;
					}
					else {
						i = *start + 1;
						j = 0;
					}
				}
				else if (buffer[i] >= 97 && buffer[i] <= 122) {
					if (buffer[i] == findWhat[j] || buffer[i] - 32 == findWhat[j]) {
						i++;
						j++;
					}
					else {
						i = *start + 1;
						j = 0;
					}
				}
				else if (buffer[i] == findWhat[j]) {
					i++;
					j++;
				}
				else {
					i = *start + 1;
					j = 0;
				}
			}
			if (findWhat[j] == '\0') {
				column = 0;
				*found = true;
				*row = line;
				k = 0;
				while (k < *start) {
					if (buffer[k] >= 9 && buffer[k] < 127) {
						column++;
					}
					else {
						k++;
						column++;
					}
					k++;
				}
				*start = column;
				while (k < i) {
					if (buffer[k] >= 9 && buffer[k] < 127) {
						column++;
					}
					else {
						k++;
						column++;
					}
					k++;
				}
				*end = column;
			}
			line++;
		}
		fclose(file);
	}
}

void Document::FindNextMatchWholeWord(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm){
	char buffer[MAX];
	bool match = false;
	Long i;
	Long j;
	Long k;
	Long byte;
	Long line = startRow;
	Long column;
	FILE* file;

	*found = false;
	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (line <= this->length && *found == false) {
			fseek(file, (line - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			i = 0;
			if (line != startRow) {
				j = 0;
			}
			else {
				j = this->GetIndex(buffer, startColumn);
			}
			while (i < byte && findWhat[j] != '\0') {
				if (j == 0) {
					*start = i;
				}
				if (buffer[i] >= 65 && buffer[i] <= 90) {
					if (buffer[i] == findWhat[j] || buffer[i] + 32 == findWhat[j]) {
						i++;
						j++;
					}
					else {
						i = *start + 1;
						j = 0;
					}
				}
				else if (buffer[i] >= 97 && buffer[i] <= 122) {
					if (buffer[i] == findWhat[j] || buffer[i] - 32 == findWhat[j]) {
						i++;
						j++;
					}
					else {
						i = *start + 1;
						j = 0;
					}
				}
				else if (buffer[i] == findWhat[j]) {
					i++;
					j++;
				}
				else {
					i = *start + 1;
					j = 0;
				}
				if (findWhat[j] == '\0') {
					if (*start == 0) {
						match = true;
					}
					else if (buffer[*start - 1] == ' ') {
						match = true;
					}
					if (match) {
						match = false;
						if (i == byte) {
							match = true;
						}
						else if (buffer[i] == ' ') {
							match = true;
						}
					}
					if (match == false) {
						j = 0;
					}
				}
			}
			if (match == true && findWhat[j] == '\0') {
				column = 0;
				*found = true;
				*row = line;
				k = 0;
				while (k < *start) {
					if (buffer[k] >= 9 && buffer[k] < 127) {
						column++;
					}
					else {
						k++;
						column++;
					}
					k++;
				}
				*start = column;
				while (k < i) {
					if (buffer[k] >= 9 && buffer[k] < 127) {
						column++;
					}
					else {
						k++;
						column++;
					}
					k++;
				}
				*end = column;
			}
			line++;
		}
		fclose(file);
	}
}

void Document::FindNextMatchCase(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm){
	char buffer[MAX];
	Long i;
	Long j;
	Long k;
	Long byte;
	Long line = startRow;
	Long column;
	FILE* file;

	*found = false;
	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (line <= this->length && *found == false) {
			fseek(file, (line - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			i = 0;
			if (line != startRow) {
				j = 0;
			}
			else {
				j = this->GetIndex(buffer, startColumn);
			}
			while (i < byte && findWhat[j] != '\0') {
				if (j == 0) {
					*start = i;
				}
				if (buffer[i] != findWhat[j]) {
					i = *start + 1;
					j = 0;
				}
				else {
					i++;
					j++;
				}
			}
			if (findWhat[j] == '\0') {
				column = 0;
				*found = true;
				*row = line;
				k = 0;
				while (k < *start) {
					if (buffer[k] >= 9 && buffer[k] < 127) {
						column++;
					}
					else {
						k++;
						column++;
					}
					k++;
				}
				*start = column;
				while (k < i) {
					if (buffer[k] >= 9 && buffer[k] < 127) {
						column++;
					}
					else {
						k++;
						column++;
					}
					k++;
				}
				*end = column;
			}
			line++;
		}
		fclose(file);
	}
}

void Document::FindNextMatchWholeWordAndCase(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm){
	char buffer[MAX];
	bool match = false;
	Long i;
	Long j;
	Long k;
	Long byte;
	Long line = startRow;
	Long column;
	FILE* file;

	*found = false;
	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (line <= this->length && *found == false) {
			fseek(file, (line - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			i = 0;
			if (line != startRow) {
				j = 0;
			}
			else {
				j = this->GetIndex(buffer, startColumn);
			}

			while (i < byte && findWhat[j] != '\0') {
				if (j == 0) {
					*start = i;
				}
				if (buffer[i] != findWhat[j]) {
					i = *start + 1;
					j = 0;
				}
				else {
					i++;
					j++;
				}
				if (findWhat[j] == '\0') {
					if (*start == 0) {
						match = true;
					}
					else if (buffer[*start - 1] == ' ') {
						match = true;
					}
					if (match) {
						match = false;
						if (i == byte) {
							match = true;
						}
						else if (buffer[i] == ' ') {
							match = true;
						}
					}
					if (match == false) {
						j = 0;
					}
				}
			}
			if (match == true && findWhat[j] == '\0') {
				column = 0;
				*found = true;
				*row = line;
				k = 0;
				while (k < *start) {
					if (buffer[k] >= 9 && buffer[k] < 127) {
						column++;
					}
					else {
						k++;
						column++;
					}
					k++;
				}
				*start = column;
				while (k < i) {
					if (buffer[k] >= 9 && buffer[k] < 127) {
						column++;
					}
					else {
						k++;
						column++;
					}
					k++;
				}
				*end = column;
			}
			line++;
		}
		fclose(file);
	}
}

void Document::FindPrevious(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm){
	char buffer[MAX];
	Long i;
	Long j;
	Long k;
	Long byte;
	Long line = startRow;
	Long column = 0;
	FILE* file;

	*found = false;
	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (line >= 1 && *found == false) {
			fseek(file, (line - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			if (line != startRow) {
				i = byte - 1;
			}
			else {
				i = this->GetIndex(buffer, startColumn) - 1;
			}
			j = strlen(findWhat) - 1;
			while (i >= 0 && j >= 0) {
				if (j == strlen(findWhat) - 1) {
					*end = i;
				}
				if (buffer[i] >= 65 && buffer[i] <= 90) {
					if (buffer[i] == findWhat[j] || buffer[i] + 32 == findWhat[j]) {
						i--;
						j--;
					}
					else {
						i = *end - 1;
						j = strlen(findWhat) - 1;
					}
				}
				else if (buffer[i] >= 97 && buffer[i] <= 122) {
					if (buffer[i] == findWhat[j] || buffer[i] - 32 == findWhat[j]) {
						i--;
						j--;
					}
					else {
						i = *end - 1;
						j = strlen(findWhat) - 1;
					}
				}
				else if (buffer[i] == findWhat[j]) {
					i--;
					j--;
				}
				else {
					i = *end - 1;
					j = strlen(findWhat) - 1;
				}
			}
			if (j < 0) {
				i++;
				*found = true;
				*row = line;
				k = 0;
				while (k < i) {
					if (buffer[k] < 0 || buffer[k] > 127) {
						k++;
					}
					column++;
					k++;
				}
				*start = column;
				while (k <= *end) {
					if (buffer[k] < 0 || buffer[k] > 127) {
						k++;
					}
					column++;
					k++;
				}
				*end = column;
			}
			line--;
		}
		fclose(file);
	}
}

void Document::FindPreviousMatchCase(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm){
	char buffer[MAX];
	Long i;
	Long j;
	Long k;
	Long byte;
	Long line = startRow;
	Long column = 0;
	FILE* file;

	*found = false;
	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (line >= 1 && *found == false) {
			fseek(file, (line - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			if (line != startRow) {
				i = byte - 1;
			}
			else {
				i = this->GetIndex(buffer, startColumn) - 1;
			}
			j = strlen(findWhat) - 1;
			while (i >= 0 && j >= 0) {
				if (j == strlen(findWhat) - 1) {
					*end = i;
				}
				
				if (buffer[i] != findWhat[j]) {
					i = *end - 1;
					j = strlen(findWhat) - 1; 
				}
				else {
					i--;
					j--;
				}
			}
			if (j < 0) {
				i++;
				*found = true;
				*row = line;
				k = 0;
				while (k < i) {
					if (buffer[k] < 0 || buffer[k] > 127) {
						k++;
					}
					column++;
					k++;
				}
				*start = column;
				while (k <= *end) {
					if (buffer[k] < 0 || buffer[k] > 127) {
						k++;
					}
					column++;
					k++;
				}
				*end = column;
			}
			line--;
		}
		fclose(file);
	}
}

void Document::FindPreviousMatchWholeWord(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm){
	char buffer[MAX];
	bool match = false;
	Long i;
	Long j;
	Long k;
	Long byte;
	Long line = startRow;
	Long column = 0;
	FILE* file;

	*found = false;
	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (line >= 1 && *found == false) {
			fseek(file, (line - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			if (line != startRow) {
				i = byte - 1;
			}
			else {
				i = this->GetIndex(buffer, startColumn) - 1;
			}
			j = strlen(findWhat) - 1;
			while (i >= 0 && j >= 0) {
				if (j == strlen(findWhat) - 1) {
					*end = i;
				}
				if (buffer[i] >= 65 && buffer[i] <= 90) {
					if (buffer[i] == findWhat[j] || buffer[i] + 32 == findWhat[j]) {
						i--;
						j--;
					}
					else {
						i = *end - 1;
						j = strlen(findWhat) - 1;
					}
				}
				else if (buffer[i] >= 97 && buffer[i] <= 122) {
					if (buffer[i] == findWhat[j] || buffer[i] - 32 == findWhat[j]) {
						i--;
						j--;
					}
					else {
						i = *end - 1;
						j = strlen(findWhat) - 1;
					}
				}
				else if (buffer[i] == findWhat[j]) {
					i--;
					j--;
				}
				else {
					i = *end - 1;
					j = strlen(findWhat) - 1;
				}
				if (j < 0) {
					if (i + 1 == 0) {
						match = true;
					}
					else if (buffer[i] == ' ') {
						match = true;
					}
					if (match) {
						match = false;
						if (*end == byte - 1) {
							match = true;
						}
						else if(buffer[*end + 1] == ' ') {
							match = true;
						}
					}
				}
			}
			if (match) {
				i++;
				*found = true;
				*row = line;
				k = 0;
				while (k < i) {
					if (buffer[k] < 0 || buffer[k] > 127) {
						k++;
					}
					column++;
					k++;
				}
				*start = column;
				while (k <= *end) {
					if (buffer[k] < 0 || buffer[k] > 127) {
						k++;
					}
					column++;
					k++;
				}
				*end = column;
			}
			line--;
		}
		fclose(file);
	}
}

void Document::FindPreviousMatchWholeWordAndCase(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm){
	char buffer[MAX];
	bool match = false;
	Long i;
	Long j;
	Long k;
	Long byte;
	Long line = startRow;
	Long column = 0;
	FILE* file;

	*found = false;
	file = fopen(fm->GetFile(), "rt");
	if (file != NULL) {
		while (line >= 1 && *found == false) {
			fseek(file, (line - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(buffer, byte, 1, file);
			if (line != startRow) {
				i = byte - 1;
			}
			else {
				i = this->GetIndex(buffer, startColumn) - 1;
			}
			j = strlen(findWhat) - 1;
			while (i >= 0 && j >= 0) {
				if (j == strlen(findWhat) - 1) {
					*end = i;
				}
				if (buffer[i] != findWhat[j]) {
					i = *end - 1;
					j = strlen(findWhat) - 1;
				}
				else {
					i--;
					j--;
				}
				if (j < 0) {
					if (i + 1 == 0) {
						match = true;
					}
					else if (buffer[i] == ' ') {
						match = true;
					}
					if (match) {
						match = false;
						if (*end == byte - 1) {
							match = true;
						}
						else if (buffer[*end + 1] == ' ') {
							match = true;
						}
					}
				}
			}
			if (match) {
				i++;
				*found = true;
				*row = line;
				k = 0;
				while (k < i) {
					if (buffer[k] < 0 || buffer[k] > 127) {
						k++;
					}
					column++;
					k++;
				}
				*start = column;
				while (k <= *end) {
					if (buffer[k] < 0 || buffer[k] > 127) {
						k++;
					}
					column++;
					k++;
				}
				*end = column;
			}
			line--;
		}
		fclose(file);
	}
}

bool Document::Compare(char* findWhat, Position start, Position end, bool matchCase, FileMaker* fm) {
	bool found = false;
	char buffer[MAX];
	char contents[MAX];
	Long i;
	Long j = 0;
	Long k = 0;
	Long byte;
	Long findWhatLength;
	Long bufferLength;
	FILE* file;
	Glyph* row = 0;
	Glyph* character = 0;

	if (this->start <= start.row && this->start + this->paper->GetRowCount() - 1 >= end.row) {
		i = this->start;
		while (i < start.row) {
			row = this->paper->GetAt(j);
			if (!(dynamic_cast<DummyRow*>(row))) {
				i++;
			}
			j++;
		}
		i = 0;
		row = this->paper->GetAt(j);
		while (i < row->GetLength() && found == false) {
			character = row->GetAt(i);
			if (character->GetIsSelected()) {
				found = true;
			}
			else {
				i++;
			}
			if (i >= row->GetLength()) {
				j++;
				row = this->paper->GetAt(j);
				i = 0;
			}
		}
		while (i < row->GetLength() && found) {
			character = row->GetAt(i);
			if (character->GetIsSelected()) {
				if (dynamic_cast<SingleByteCharacter*>(character)) {
					buffer[k] = character->GetCharacter();
				}
				else {
					buffer[k] = character->GetCharacters()[0];
					k++;
					buffer[k] = character->GetCharacters()[1];
				}
				k++;
			}
			else {
				found = false;
			}
			i++;
			if (i >= row->GetLength() && found) {
				if (j + 1< this->paper->GetLength() && dynamic_cast<DummyRow*>(this->paper->GetAt(j + 1))) {
					j++;
					row = this->paper->GetAt(j);
					i = 0;
				}
			}
		}
	}
	else {
		file = fopen(fm->GetFile(), "rt");
		if (file != NULL) {
			fseek(file, (start.row - 1) * (12 + MAX), SEEK_SET);
			fscanf(file, "%d", &byte);
			fseek(file, 8, SEEK_CUR);
			fread(contents, byte, 1, file);
			i = this->GetIndex(contents, start.column);
			j = this->GetIndex(contents, end.column);
			while (i < j) {
				buffer[k] = contents[i];
				k++;
				i++;
			}
			fclose(file);
		}
	}
	buffer[k] = '\0';
	findWhatLength = strlen(findWhat);
	bufferLength = strlen(buffer);
	if (findWhatLength == bufferLength) {
		found = true;
		i = 0;
		if (matchCase == false) {
			while (i < findWhatLength && found) {
				if (buffer[i] >= 65 && buffer[i] <= 90) {
					if (buffer[i] == findWhat[i] || buffer[i] + 32 == findWhat[i]) {
						i++;
					}
					else {
						found = false;
					}
				}
				else if (buffer[i] >= 97 && buffer[i] <= 122) {
					if (buffer[i] == findWhat[i] || buffer[i] - 32 == findWhat[i]) {
						i++;
					}
					else {
						found = false;
					}
				}
				else if (findWhat[i] == buffer[i]) {
					i++;
				}
				else {
					found = false;
				}
			}
		}
		else {
			while (i < findWhatLength && found) {
				if (findWhat[i] == buffer[i]) {
					i++;
				}
				else {
					found = false;
				}
			}
		}
	}
	else {
		found = false;
	}

	return found;
}

void Document::FindNext(char* buffer, Long index, char* findWhat, bool* found, Long* start, Long* end){
	Long i = 0;
	Long j = index;
	
	*found = false;
	*start = -1;
	*end = -1;

	while (findWhat[i] != '\0' && buffer[j] != '\0') {
		if (i == 0) {
			index = j;
		}
		if (findWhat[i] >= 0 && findWhat[i] < ASCII) {
			if (findWhat[i] >= 65 && findWhat[i] <= 90) {
				if (findWhat[i] == buffer[j] || findWhat[i] + 32 == buffer[j]) {
					i++;
					j++;
				}
				else {
					i = 0;
					j = index + 1;
				}
			}
			else if (findWhat[i] >= 97 && findWhat[i] <= 122) {
				if (findWhat[i] == buffer[j] || findWhat[i] - 32 == buffer[j]) {
					i++;
					j++;
				}
				else {
					i = 0;
					j = index + 1;
				}
			}
			else if (findWhat[i] == buffer[j]) {
				i++;
				j++;
			}
			else {
				i = 0;
				j = index + 1;
			}
		}
		else {
			if (findWhat[i] == buffer[j] && findWhat[i + 1] == buffer[j + 1]) {
				i += 2;
				j += 2;
			}
			else {
				i = 0;
				j = index + 1;
			}
		}
	}
	if (findWhat[i] == '\0') {
		*found = true;
		*start = index;
		*end = j - 1;
	}
}

void Document::FindNextMatchWholeWord(char* buffer, Long index, char* findWhat, bool* found, Long* start, Long* end){
	bool match = false;
	Long i = 0;
	Long j = index;
	
	*found = false;
	*start = -1;
	*end = -1;
	
	while (findWhat[i] != '\0' && buffer[j] != '\0') {
		if (i == 0) {
			index = j;
		}
		if (findWhat[i] >= 0 && findWhat[i] < ASCII) {
			if (findWhat[i] >= 65 && findWhat[i] <= 90) {
				if (findWhat[i] == buffer[j] || findWhat[i] + 32 == buffer[j]) {
					i++;
					j++;
				}
				else {
					i = 0;
					j = index + 1;
				}
			}
			else if (findWhat[i] >= 97 && findWhat[i] <= 122) {
				if (findWhat[i] == buffer[j] || findWhat[i] - 32 == buffer[j]) {
					i++;
					j++;
				}
				else {
					i = 0;
					j = index + 1;
				}
			}
			else if (findWhat[i] == buffer[j]) {
				i++;
				j++;
			}
			else {
				i = 0;
				j = index + 1;
			}
		}
		else {
			if (findWhat[i] == buffer[j] && findWhat[i + 1] == buffer[j + 1]) {
				i += 2;
				j += 2;
			}
			else {
				i = 0;
				j = index + 1;
			}
		}
		if (findWhat[i] == '\0') {
			match = false;
			if (index == 0) {
				match = true;
			}
			else {
				if (buffer[index - 1] == ' ') {
					match = true;
				}
			}
			if (match) {
				match = false;
				if (buffer[j] == '\0') {
					match = true;
				}
				else {
					if (buffer[j] == ' ') {
						match = true;
					}
				}
			}
		}
	}
	if (match) {
		*found = true;
		*start = index;
		*end = j - 1;
	}
}

void Document::FindNextMatchCase(char* buffer, Long index, char* findWhat, bool* found, Long* start, Long* end){
	Long i = 0;
	Long j = index;

	*found = false;
	*start = -1;
	*end = -1;

	while (findWhat[i] != '\0' && buffer[j] != '\0') {
		if (i == 0) {
			index = j;
		}
		if (findWhat[i] >= 0 && findWhat[i] < ASCII) {
			if (findWhat[i] == buffer[j]) {
				i++;
				j++;
			}
			else {
				i = 0;
				j = index + 1;
			}
		}
		else {
			if (findWhat[i] == buffer[j] && findWhat[i + 1] == buffer[j + 1]) {
				i += 2;
				j += 2;
			}
			else {
				i = 0;
				j = index + 1;
			}
		}
	}
	if (findWhat[i] == '\0') {
		*found = true;
		*start = index;
		*end = j - 1;
	}
}

void Document::FindNextMatchWholeWordAndCase(char* buffer, Long index, char* findWhat, bool* found, Long* start, Long* end){
	bool match = false;
	Long i = 0;
	Long j = index;

	*found = false;
	*start = -1;
	*end = -1;

	while (findWhat[i] != '\0' && buffer[j] != '\0') {
		if (i == 0) {
			index = j;
		}
		if (findWhat[i] >= 0 && findWhat[i] < ASCII) {
			if (findWhat[i] == buffer[j]) {
				i++;
				j++;
			}
			else {
				i = 0;
				j = index + 1;
			}
		}
		else {
			if (findWhat[i] == buffer[j] && findWhat[i + 1] == buffer[j + 1]) {
				i += 2;
				j += 2;
			}
			else {
				i = 0;
				j = index + 1;
			}
		}
		if (findWhat[i] == '\0') {
			match = false;
			if (index == 0) {
				match = true;
			}
			else {
				if (buffer[index - 1] == ' ') {
					match = true;
				}
			}
			if (match) {
				match = false;
				if (buffer[j] == '\0') {
					match = true;
				}
				else {
					if (buffer[j] == ' ') {
						match = true;
					}
				}
			}
		}
	}
	if (match) {
		*found = true;
		*start = index;
		*end = j - 1;
	}
}

void Document::Correct(char* buffer, Long start, Long end, CharacterMetrics* characterMetrics, char* contents, char* replaceWith, Long* byte, Long* rowLength, Long* index){
	Long i = 0;
	Long j = 0;

	*rowLength = 0;
	while (i < start) {
		contents[i] = buffer[i];
		i++;
	}
	while (replaceWith[j] != '\0') {
		contents[i] = replaceWith[j];
		i++;
		j++;
	}
	*index = i;
	j = end + 1;
	while (i < MAX - 2 && buffer[j] != '\0') {
		contents[i] = buffer [j];
		i++;
		j++;
	}
	*byte = i;
	while (i < MAX - 2) {
		contents[i] = 127;
		i++;
	}
	i = 0;
	while (i < *byte) {
		if (contents[i] >= 0 && contents[i] < ASCII) {
			*rowLength += characterMetrics->GetWidth(contents[i]);
		}
		else {
			i++;
			*rowLength += characterMetrics->GetWidth(128);
		}
		i++;
	}
}