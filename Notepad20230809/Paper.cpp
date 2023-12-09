//Paper.cpp

#include "Paper.h"
#include "Glyph.h"
#include "Row.h"
#include "DummyRow.h"
#include "MultiByteCharacter.h"
#include "SingleByteCharacter.h"
#include "GlyphVisitor.h"
#include "GlyphFactory.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "Position.h"
#include "resource.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Ŭ������: Paper
��    ��: Paper -�� Composite -�� Glyph / Paper ���̿� ���� Ŭ����(Concrete Class)
*/

Paper::Paper(Long capacity)
	:Composite(capacity){
	this->rowCount = 0;
}

Paper::Paper(const Paper& source)
	: Composite(source){
	this->rowCount = source.rowCount;
}

Paper::~Paper() {

}

Long Paper::Add(Glyph* glyph) {
	Long index;

	if (this->length < this->capacity) {
		index = this->glyphs.Store(this->length, glyph);
	}

	else {
		index = this->glyphs.AppendFromRear(glyph);
		this->capacity++;
	}

	this->length++;
	this->current = index + 1;

	if (!(dynamic_cast<DummyRow*>(glyph))) {
		this->rowCount++;
	}
	
	return index;
}

Long Paper::Add(Long index, Glyph* glyph) {

	index = this->glyphs.Insert(index, glyph);

	this->capacity++;
	this->length++;
	this->current = index + 1;

	if (!(dynamic_cast<DummyRow*>(glyph))) {
		this->rowCount++;
	}

	return index;
}

Long Paper::Remove(Long index) {

	this->current = index;

	if (this->current < 1) {
		this->current = 1;
	}

	if (!(dynamic_cast<DummyRow*>(this->glyphs[index]))) {
		this->rowCount--;
	}

	if (this->glyphs[index] != 0) {
		delete this->glyphs[index];
	}

	index = this->glyphs.Delete(index);

	this->capacity--;
	this->length--;

	if (this->current > this->length) {
		this->current = this->length;
	}

	return index;
}

void Paper::Delete(Long start, Position startPosition, Position endPosition, bool isWrapped, CharacterMetrics* characterMetrics, Long width) {
	Long i = startPosition.row - start + 1;
	Long j;
	Long k;
	Long index;
	Long count;
	Glyph* startRow;
	Glyph* endRow;
	Glyph* character;
	
	this->current = i;
	if (isWrapped) {
		i = this->GetRowNumber(start, startPosition.row);
		this->current = i;
		this->RowUnWrap(characterMetrics);
	}
	startRow = this->GetAt(i - 1);
	if (startPosition.row == endPosition.row) {
		count = endPosition.column - startPosition.column;
		j = 1;
		while (j <= count) {
			startRow->Remove(startPosition.column, characterMetrics);
			j++;
		}
		if (isWrapped) {
			this->WrapRow(i - 1, width, characterMetrics, true);
		}
	}
	else {
		startRow->Move(startPosition.column);
		while (startRow->GetCurrent() < startRow->GetLength()) {
			startRow->Remove(startRow->GetCurrent(), characterMetrics);
		}
		j = endPosition.row - start + 1;
		if (isWrapped) {
			j = this->GetRowNumber(start, endPosition.row);
			this->current = j;
			this->RowUnWrap(characterMetrics);
		}
		endRow = this->GetAt(j - 1);
		index = startRow->GetCurrent();
		k = endPosition.column;
		while (k < endRow->GetLength()) {
			character = endRow->GetAt(k);
			startRow->Add(character->Clone(), characterMetrics);
			k++;
		}
		startRow->Move(index);
		count = j - i;
		k = 1;
		while (k <= count) {
			this->Remove(i);
			k++;
		}
		if (isWrapped) {
			this->WrapRow(i - 1, width, characterMetrics, true);
		}
	}
}

/*
�Լ���Ī: Clone
��    ��: ������ ��ü�� �Ȱ��� ��ü�� �����ϴ�. (���� ����������: Prototype)
��    ��: ����
��    ��: Glyph Link
*/
Glyph* Paper::Clone() {
	return new Paper(*this);
}

/*
�Լ���Ī: GetContents
��    ��: �ش� ��ü�� ������ ���ϴ�.
��    ��: ����
��    ��: string
*/
string Paper::GetContents() {
	string contents("");
	Long i = 0;

	while (i < this->length) {
		contents += this->glyphs[i]->GetContents();

		//���������� �ƴϸ� ������ �ϴ�.
		if (i != this->length - 1) {
			contents += "\n";
		}
		
		i++;
	}

	return contents;
}

/*
�Լ���Ī: Accept
��    ��: �ԷµǴ� GlyphVisitor�� ConcreteŬ������ ���� �ش� Ŭ������ Visit�Լ��� ȣ���Ѵ�.
��    ��: GlyphVisitor Link
��    ��: ����
*/
void Paper::Accept(GlyphVisitor* visitor) {
	visitor->Visit(this);
}

Long Paper::First() {

	this->current = 1;

	return this->current;
}

Long Paper::Last() {

	this->current = this->length;

	return this->current;
}

Long Paper::Previous() {

	this->current--;

	if (this->current < 1) {
		this->current++;
	}

	return this->current;
}

Long Paper::Next() {
	
	this->current++;

	if (this->current > this->length) {
		this->current--;
	}

	return this->current;
}

void Paper::Wrap(int width, CharacterMetrics* characterMetrics) {
	Long i = 0;
	Long j;
	Long count;
	Long current;
	Long column;
	Glyph* row;
	Glyph* glyph;
	GlyphFactory glyphFactory;
	
	//1. ���� ��ġ�� ����ϴ�.
	current = this->current;
	row = this->GetAt(current - 1);
	column = row->GetCurrent();
	
	//2. �� ���� ���ϴ�.
	count = this->length;

	//3. �� ����ŭ �ݺ��ϴ�.
	while (i < count) {
		row = this->GetAt(0);
		if (row->GetLength() == 0) {
			glyph = row->Clone();
			this->Add(glyph);
		}
		else {
			//3.1. ���� ����ŭ �ݺ��ϴ�.
			j = 0;
			while (j < row->GetLength()) {
				//3.2. �ٿ��� �ڵ��ٹٲ��Ͽ� ���̿� �߰��ϴ�.
				row->Wrap(width, characterMetrics, &j, &glyph);
				this->Add(glyph);
			}
		}	
		//3.2. �ڵ��ٹٲ��� ���� �����.
		this->Remove(0);
		i++;
	}
	i = 1;
	j = 1;
	while (i < current) {
		row = this->GetAt(j - 1);
		j++;
		if (!(dynamic_cast<DummyRow*>(row))) {
			i++;
		}
	}
	this->current = j;
	row = this->GetAt(j - 1);
	while (dynamic_cast<DummyRow*>(row)) {
		this->Next();
		row = this->GetAt(this->current - 1);
	}
	row = this->GetAt(this->current - 1);
	while (column > row->GetLength()) {
		column -= row->GetLength();
		this->current++;
		row = this->GetAt(this->current - 1);
	}
	row->Move(column);
#if 0
	//4. ���̿��� ��ġ�� �̵��ϴ�.
	i = 0;
	j = 0;
	while (i < this->length && j < current) {
		row = this->GetAt(i);
		if (!(dynamic_cast<DummyRow*>(row))) {
			j++;
		}
		i++;
	}
	this->current = i;
	row = this->GetAt(this->current - 1);
	row->First();

	i = 0;
	while (i < column) {

		if (row->GetCurrent() < row->GetLength()) {
			row->Next();
		}

		else {
			this->Next();
			row = this->GetAt(this->current - 1);
			row->First();
			row->Next();
		}
		i++;
	}
#endif
}

void Paper::UnWrap(CharacterMetrics* characterMetrics) {
	Long i = 0;
	Long j;
	Long column;
	Long currentRow;
	Glyph* current;
	Glyph* row;
	Glyph* character;
	
	//1. ���� �� ��ġ�� �����ϴ�.
	currentRow = this->GetRowNumber(this->current);
	column = this->GetColumnNumber();

	//3. �� ����ŭ �ݺ��ϴ�.
	while (i < this->length) {
		current = this->GetAt(i);
		//3.1. ������ ���� �ƴϰ� DummyRow�� ���� �ݺ��ϴ�.
		while(i + 1 < this->length && dynamic_cast<DummyRow*>(this->GetAt(i + 1))){
			//3.1.1. ���� ����ŭ �ݺ��ϴ�.
			row = this->GetAt(i + 1);
			j = 0;
			while (j < row->GetLength()) {
				character = row->GetAt(j);
				current->Add(character->Clone(), characterMetrics);
				j++;
			}
			i++;
		}
		//3.2. ������ ���̰ų� Row�̸�
		if (i + 1 == this->length){
			i++;
		}
		else if (!(dynamic_cast<DummyRow*>(this->GetAt(i + 1)))) {
			i++;
		}
	}
	//4. DummyRow�� ��� �����.
	i = this->length - 1;
	while (i >= 0) {
		if (dynamic_cast<DummyRow*>(this->GetAt(i))) {
			this->Remove(i);
		}
		i--;
	}
	//5. ���� ���� ��ġ�� �̵��ϴ�.
	this->current = currentRow;
	row = this->GetAt(currentRow - 1);
	row->Move(column);
}

/*
�Լ���Ī: RowWrap
��    ��: �� ������ �ڵ��ٹٲ��� �ϴ�.
��    ��: Ŭ���̾�Ʈ ����ũ��, CharacterMetrics Link
��    ��: ����
*/
void Paper::RowWrap(Long width, CharacterMetrics* characterMetrics) {
	Long i = 0;
	Glyph* character;
	Glyph* row;
	Glyph* current = this->GetAt(this->current - 1); // ���� ���� ���ϴ�.
	GlyphFactory glyphFactory;
	Long currentRow = this->current; // ���̿��� ������ġ�� �����ϴ�.
	Long currentColumn = current->GetCurrent(); // �ٿ��� ������ġ�� �����ϴ�.
	Long rowWidth = 0;

	//1. Row�� �����ϴ�.
	row = glyphFactory.Create((char*)"\r\n");

	//2. ���� ���� ���û����̸� �����ϴ�.
	if (current->GetIsSelected() == true) {
		row->Select();
	}

	//3. ���̿��� �߰��ϴ�.
	this->Add(this->current, row);

	//4. ���� ���� ���� ����ŭ �ݺ��ϴ�.
	while (i < current->GetLength()) {

		//4.1. ���ڸ� ���ϴ�.
		character = current->GetAt(i);

		//4.2. �ٱ��̿� ���� ���� ���� Ŭ���̾�Ʈ ���α��̺��� �۰ų� ������
		if (rowWidth + character->GetWidth(characterMetrics) <= width) {

			//4.2.1. �ٿ��� ���ڸ� �߰��ϴ�.
			row->Add(character->Clone(), characterMetrics);

			//4.2.2. �ٱ��̸� ���ϴ�.
			rowWidth += character->GetWidth(characterMetrics);
		}

		//4.3. �ٱ��̿� ���� ���� ���� Ŭ���̾�Ʈ ���α��� ���� ũ��
		else {

			//4.3.1. DummyRow�� �����.
			row = glyphFactory.Create((char*)"\r");

			//4.3.2. ���̿��� �߰��ϴ�.
			this->Add(this->current, row);

			//4.3.3. �� ���̸� ����ġ�ϴ�.
			rowWidth = 0;

			//4.4.4. �ٿ��� ���ڸ� �߰��ϴ�.
			row->Add(character->Clone(), characterMetrics);

			//4.4.5. �� ���̸� ���ϴ�.
			rowWidth += character->GetWidth(characterMetrics);
		}
		i++;
	}

	//5. �ڵ��ٹٲ� �Ϸ�� ���� �����.
	this->Remove(currentRow - 1);
	if (currentRow > 1) {
		this->current++;
	}
	
	current = this->GetAt(this->current -1);
	

	//6. �ڵ��ٹٲ� �� ������ġ�� �ڵ��ٹٲ� �� ���� ��ġ�� �̵��ϴ�.
	while (currentColumn > current->GetLength()) {
		currentColumn -= current->GetLength();
		this->Next();
		current = this->GetAt(this->current - 1);
	}
	
	current->First();
	i = 0;
	
	while (i < currentColumn) {
		current->Next();
		i++;
	}
}

void Paper::WrapRow(Long index, Long width, CharacterMetrics* characterMetrics, bool move) {
	Glyph* row;
	Glyph* current;
	Glyph* character;
	GlyphFactory glyphFactory;
	Long i = 0;
	Long column;
	Long length;

	row = glyphFactory.Create((char*)"\r\n");
	current = this->GetAt(index);
	column = current->GetCurrent();
	if (current->GetIsSelected()) {
		row->Select();
	}
	this->Add(index, row);
	while (i < current->GetLength()) {
		character = current->GetAt(i);
		if (row->GetRowLength() + character->GetWidth(characterMetrics) <= width) {
			row->Add(character->Clone(), characterMetrics);
		}
		else {
			row = glyphFactory.Create((char*)"\r");
			this->Add(this->current, row);
			row->Add(character->Clone(), characterMetrics);
		}
		i++;
	}
	this->Remove(this->current);
	if (move == true) {
		this->current = index + 1;
		current = this->GetAt(this->current - 1);
		while (column > current->GetLength()) {
			length = current->GetLength();
			this->current++;
			current = this->GetAt(this->current - 1);
			column -= length;
		}
		current->Move(column);
	}
}

/*
�Լ���Ī: RowUnWrap
��    ��: �� ������ �ڵ��ٹٲ��� �ϴ�.
��    ��: Ŭ���̾�Ʈ ����ũ��, CharacterMetrics Link
��    ��: ����
*/
void Paper::RowUnWrap(CharacterMetrics* characterMetrics) {
	Long index;
	Glyph* nextRow;
	Glyph* character;
	Glyph* current = this->GetAt(this->current - 1); // ���� ���� ���ϴ�.
	Long column = current->GetCurrent(); // �ٿ��� ���� ĭ�� ���ϴ�.
	
	//1. ���� ���� DummyRow�� ���� �ݺ��ϴ�.
	while (dynamic_cast<DummyRow*>(current)) {
		//1.1. ���̿��� �������� �̵��ϴ�.
		index = this->Previous();
		//1.2. ���� ���� ���ϴ�.
		current = this->GetAt(index - 1);
		//1.3. ĭ�� ���ϴ�.
		column += current->GetLength();
	}

	//2. ������ ���� �ƴϰ� �������� DummyRow�� ���� �ݺ��ϴ�.
	while (this->current < this->length && dynamic_cast<DummyRow*>(this->GetAt(this->current))) {
		//2.1. index�� ����ġ�ϴ�.
		index = 0;
		//2.2. ���� ���� ���ϴ�.
		nextRow = this->GetAt(this->current);
		//2.3. ���� �� ���ڼ���ŭ �ݺ��ϴ�.
		while (index < nextRow->GetLength()) {
			//2.3.1. ���ڸ� ���ϴ�.
			character = nextRow->GetAt(index);
			//2.3.2. �ٿ� ���ڸ� �߰��ϴ�.
			current->Add(character->Clone(), characterMetrics);
			//2.3.3. index�� ����.
			index++;
		}
		//2.4. �� �Ű� ���� ���� �����.
		this->Remove(this->current);
	}
	//3. �����ٿ��� �̵��ϴ�.
	current->Move(column);
}

void Paper::UnWrapRow(Long index, CharacterMetrics* characterMetrics) {
	Long i = index;
	Long j;
	Long count = 0;
	Long current = this->current;
	Glyph* row;
	Glyph* next;
	Glyph* character;
	
	row = this->GetAt(i - 1);
	while (dynamic_cast<DummyRow*>(row)) {
		i--;
		row = this->GetAt(i - 1);
	}
	index = i;
	while (i < this->GetLength() && dynamic_cast<DummyRow*>(this->GetAt(i))) {
		next = this->GetAt(i);
		j = 0;
		while (j < next->GetLength()) {
			character = next->GetAt(j);
			row->Add(character->Clone(), characterMetrics);
			j++;
		}
		i++;
	}
	i = index;
	while (i < this->GetLength() && dynamic_cast<DummyRow*>(this->GetAt(i))) {
		count++;
		this->Remove(i);
	}
	if (index < current) {
		current -= count;
	}
	this->current = current;
}


Long Paper::GetRowNumber(Long index) {
	Long i = 0;
	Long count = 0;
	Glyph* row;

	while (i < index) {
		row = this->GetAt(i);
		if (!dynamic_cast<DummyRow*>(row)) {
			count++;
		}
		i++;
	}
	return count;
}

Long Paper::GetCloseRow(Long index) {
	Glyph* row;

	row = this->GetAt(index - 1);
	while (dynamic_cast<DummyRow*>(row)) {
		index--;
		row = this->GetAt(index - 1);
	}

	return index;
}

Long Paper::GetRowNumber(Long start, Long index) {
	Long i = 0;
	Long row = start - 1;
	Glyph* glyph;

	while (row < index) {
		glyph = this->GetAt(i);
		if (!(dynamic_cast<DummyRow*>(glyph))) {
			row++;
		}
		i++;
	}
	return i;
}


Long Paper::GetColumnNumber() {
	Long index = this->current;
	Long column;
	Glyph* row;

	row = this->GetAt(index - 1);
	column = row->GetCurrent();
	while (dynamic_cast<DummyRow*>(row)) {
		index--;
		row = this->GetAt(index - 1);
		column += row->GetLength();
	}
	return column;
}

void Paper::Select() {
	Long i = 0;
	Long j;
	Glyph* row;
	Glyph* character;

	while (i < this->length) {
		row = this->GetAt(i);
		j = 0;
		while (j < row->GetLength()) {
			character = row->GetAt(j);
			character->Select();
			j++;
		}
		if (i > 0) {
			row->Select();
		}
		i++;
	}
}

void Paper::UnSelect(){
	bool upDown = true;
	bool isSelecting = true;
	Long i = this->current;
	Long j;
	Glyph* row = this->GetAt(this->current - 1);
	Glyph* character;

	if (row->GetLength() > 0) {
		if (row->GetCurrent() < row->GetLength()) {
			character = row->GetAt(row->GetCurrent());
			if (character->GetIsSelected()) {
				upDown = false;
			}
		}
		else {
			character = row->GetAt(row->GetCurrent() - 1);
			if (character->GetIsSelected() == false) {
				upDown = false;
			}
		}
	}
	else {
		if (row->GetIsSelected() == false) {
			upDown = false;
		}
	}
	if (upDown) {
		j = row->GetCurrent() - 1;
		while (j >= 0 && isSelecting) {
			character = row->GetAt(j);
			if (character->GetIsSelected()) {
				character->UnSelect();
			}
			else {
				isSelecting = false;
			}
			j--;
		}
		if (!(dynamic_cast<DummyRow*>(row))){
			if (row->GetIsSelected()) {
				row->UnSelect();
			}
			else {
				isSelecting = false;
			}
		}
		i--;
		while (i >= 1 && isSelecting) {
			row = this->GetAt(i - 1);
			j = row->GetLength() - 1;
			while (j >= 0 && isSelecting) {
				character = row->GetAt(j);
				if (character->GetIsSelected()) {
					character->UnSelect();
				}
				else {
					isSelecting = false;
				}
				j--;
			}
			if (!(dynamic_cast<DummyRow*>(row)) && row->GetIsSelected()) {
				row->UnSelect();
			}
			else if (!(dynamic_cast<DummyRow*>(row)) && row->GetIsSelected() == false) {
				isSelecting = false;
			}
			i--;
		}
	}
	else {
		j = row->GetCurrent();
		while (j < row->GetLength() && isSelecting) {
			character = row->GetAt(j);
			if (character->GetIsSelected()) {
				character->UnSelect();
			}
			else {
				isSelecting = false;
			}
			j++;
		}
		i++;
		while (i <= this->length && isSelecting) {
			row = this->GetAt(i - 1);
			if ((!(dynamic_cast<DummyRow*>(row)) && row->GetIsSelected()) || dynamic_cast<DummyRow*>(row)) {
				if (!(dynamic_cast<DummyRow*>(row))) {
					row->UnSelect();
				}
				j = 0;
				while (j < row->GetLength() && isSelecting) {
					character = row->GetAt(j);
					if (character->GetIsSelected()) {
						character->UnSelect();
					}
					else {
						isSelecting = false;
					}
					j++;
				}
			}
			else {
				isSelecting = false;
			}
			i++;
		}
	}
}

int Paper::CompareSelectingPart(string findWhat) {
	Long i;
	Long j = 0;
	Long startRow;
	string selectingPart;
	Glyph* row;
	Glyph* character;
	int ret = -1;
	bool isSelecting = true;
	bool compare = true;
	
	i = this->current;

	//1. ���� Row�̸鼭 ���û����̰ų� DummyRow�̸鼭 ù��° ���ڰ� ���û����̸� i�� ����.
	while ((!(dynamic_cast<DummyRow*>(this->GetAt(i - 1))) && this->GetAt(i - 1)->GetIsSelected() == true) ||
		(dynamic_cast<DummyRow*>(this->GetAt(i - 1)) && this->GetAt(i - 2)->GetAt(this->GetAt(i - 2)->GetLength() - 1)->GetIsSelected() == true)) {
		i--;
	}

	//2. ���� ���ϴ�.
	row = this->GetAt(i - 1);

	//3. �ٿ� ���ڰ� ������
	if (row->GetLength() > 0) {
		
		//3.1. ������ĭ�� �ƴϰ� ���ڰ� �̼��� ������ ���� �ݺ��ϴ�.
		while (j < row->GetLength() && row->GetAt(j)->GetIsSelected() == false) {
			j++;
		}
#if 0
		//3.2. ������ĭ�̸�
		if (j == row->GetLength()) {

			//3.2.1. �������� DummyRow�̸� �����ٷ� �̵��ϴ�.
			row = this->GetAt(i);
			if (dynamic_cast<DummyRow*>(row)) {
				i++;
				j = 0;
			}
		}
#endif
	}

	//4. i�� �� ������ �۰ų� ���� isSelecting�� TRUE�� ���� �ݺ��ϴ�.
	startRow = i;
	while (i <= this->length && isSelecting == true) {
		
		//4.1. �������� Row�̰� ���û����̸�
		if (!(dynamic_cast<DummyRow*>(this->GetAt(i - 1))) && this->GetAt(i - 1)->GetIsSelected() == true) {

			//4.1.1. isSelecting�� ��ġ��.
			isSelecting = false;

			//4.1.2. compare�� ��ġ��.
			compare = false;
		}

#if 0
		//4.2. i�� �������� �ƴϰ� Row�̸鼭 �̼��û����̸� isSelecting�� ��ġ��.
		else if (i != startRow && !(dynamic_cast<DummyRow*>(this->GetAt(i - 1)) && this->GetAt(i - 1)->GetIsSelected() == false)) {
			isSelecting = false;
		}

		//4.3. i�� �������� �ƴϰ� DummyRow�̸鼭 ù��° ���ڰ� �̼��� �����̸� isSelecting�� ��ġ��.
		else if (i != startRow && dynamic_cast<DummyRow*>(this->GetAt(i - 1)) && this->GetAt(i - 1)->GetAt(0)->GetIsSelected() == false) {
			isSelecting = false;
		}
#endif
		if (i > startRow) {
			j = 0;
		}
		//4.4. j�� ���ڼ����� �۰� isSelecting�� TRUE�� ���� �ݺ��ϴ�.
		while (j < this->GetAt(i - 1)->GetLength() && isSelecting == true) {
			
			character = this->GetAt(i - 1)->GetAt(j);
			
			if (character->GetIsSelected()) {
				selectingPart += character->GetContents();
			}
			
			else {
				isSelecting = false;
			}
			j++;
		}
		i++;
	}

	if (compare == true) {
		ret = findWhat.compare(selectingPart);
	}

	return ret;
}


string Paper::Copy() {
	bool copy = true;
	Long i;
	string text;
	string contents;
	Glyph* row;
	Long current = this->current;

	//1. ���� �ٺ��� �� �������� �̼��� ���� ã��.
	while (!dynamic_cast<DummyRow*>(this->GetAt(this->current - 1)) && this->GetAt(this->current - 1)->GetIsSelected() == true ||
			dynamic_cast<DummyRow*>(this->GetAt(this->current - 1)) &&
			this->GetAt(this->current - 2)->GetAt(this->GetAt(this->current - 2)->GetLength()-1)->GetIsSelected() == true){
		this->Previous();
	}

	//2. ã�� �ٺ��� ���������� �׸��� copy Flag�� true�� ���� �ݺ��ϴ�.
	i = this->current - 1;
	while (i < this->length && copy == true) {
		
		//2.1. ���� ���ϴ�.
		row = this->GetAt(i);
		//2.2. ���� Row�̰� ã�� ù��° ���̰ų�, ���� Row�̰� ���û����̸�
		if ((!(dynamic_cast<DummyRow*>(row)) && i == this->current - 1) ||
			(!(dynamic_cast<DummyRow*>(row)) && row->GetIsSelected() == true)) {

			//2.2.1. ã�� ù��° ���� �ƴϸ� text�� ���๮�ڸ� �߰��ϴ�.
			if (i > this->current - 1) {
				text += "\n";
			}
			//2.2.2. �ٿ��� �����ϴ�.
			contents = row->Copy();
		}

		//2.3. ���� Row�̰� �̼��� �����̸� ���縦 ������.
		else if (!(dynamic_cast<DummyRow*>(row)) && row->GetIsSelected() == false) {
			contents = "";
			copy = false;
		}

		//2.4. ���� DummyRow�̸�
		else if (dynamic_cast<DummyRow*>(row)) {

			//2.4.1. ���� ù��° ���ڰ� ���û����̰ų� ã�� ù��° ���̸� �ٿ��� �����ϴ�.
			if (row->GetAt(0)->GetIsSelected() == true || i == this->current - 1) {
				contents = row->Copy();
			}

			//2.4.2. ���� ù��° ���ڰ� ���û��°� �ƴϸ� ���縦 ������.
			else {
				contents = "";
				copy = false;
			}
		}

		//2.5. text�� �ٿ��� ������ ������ �߰��ϴ�.
		text += contents;

		//2.6. ���� ����.
		i++;
	}

	this->current = current;

	return text;
}

void Paper::Paste(char* text, CharacterMetrics* characterMetrics) {
	char character[2];
	Long i = 0;
	Long index;
	Glyph* row;
	Glyph* currentRow;
	Glyph* glyph;
	GlyphFactory glyphFactory;

	//1. �������� �����ϴ�.
	currentRow = this->GetAt(this->current - 1);
	row = currentRow;

	//2. �Էµ� ���ڰ� ���� �ƴ� ���� �ݺ��ϴ�.
	while (text[i] != '\0') {

		//2.1. ���๮���̸�
		if (text[i] == '\r') {
			//2.1.1. ���̿��� ���� �����.
			character[0] = text[i];
			i++;
			character[1] = text[i];
			glyph = glyphFactory.Create(character);
			if (this->current < this->length) {
				index = this->Add(this->current, glyph);
			}
			else {
				index = this->Add(glyph);
			}
			currentRow = this->GetAt(index);
		}

		//2.2. ASCII �ڵ��̸�
		else if (text[i] > 0 && text[i] < ASCII) {
			character[0] = text[i];
			character[1] = '\0';
			glyph = glyphFactory.Create(character);

			if (currentRow->GetCurrent() < currentRow->GetLength()) {
				currentRow->Add(currentRow->GetCurrent(), glyph, characterMetrics);
			}
			else if (currentRow->GetCurrent() == currentRow->GetLength()) {
				currentRow->Add(glyph, characterMetrics);
			}
		}

		//2.3. ASCII �ڵ尡 �ƴϸ�
		else if (!(text[i] > 0 && text[i] < ASCII)) {
			character[0] = text[i];
			character[1] = text[i + 1];
			glyph = glyphFactory.Create(character);

			if (currentRow->GetCurrent() < currentRow->GetLength()) {
				currentRow->Add(currentRow->GetCurrent(), glyph, characterMetrics);
			}
			else {
				currentRow->Add(glyph, characterMetrics);
			}
			i++;
		}
		i++;
	}

	if (row != currentRow) {
		i = currentRow->GetCurrent();
		while (row->GetCurrent() < row->GetLength()) {
			glyph = row->GetAt(row->GetCurrent());
			currentRow->Add(glyph->Clone(), characterMetrics);
			row->Remove(row->GetCurrent(), characterMetrics);
		}
		currentRow->Move(i);
	}
}

void Paper::FindNext(char* findWhat, bool* found, Long* row, Long* start, Long* end) {
	bool going = false;
	char one;
	char other[2];
	Long i = this->current;
	Long j;
	Long k = 0;
	Long index;
	Long rowNumber;
	Glyph* glyph = 0;
	Glyph* character;

	while (i <= this->length && findWhat[k] != '\0') {
		glyph = this->GetAt(i - 1);
		if (!(dynamic_cast<DummyRow*>(glyph))) {
			k = 0;
		}
		if (i > this->current) {
			j = 0;
		}
		else {
			j = glyph->GetCurrent();
		}
		while (j < glyph->GetLength() && findWhat[k] != '\0') {
			if (k == 0) {
				*row = i;
				*start = j;
			}
			character = glyph->GetAt(j);
			if (findWhat[k] >= 9 && findWhat[k] <= 126) {
				if (dynamic_cast<SingleByteCharacter*>(character)) {
					one = character->GetCharacter();
					if (one >= 65 && one <= 90) {
						if (one == findWhat[k] || one + 32 == findWhat[k]) {
							k++;
							going = true;
						}
						else {
							going = false;
						}
					}
					else if (one >= 97 && one <= 122) {
						if (one == findWhat[k] || one - 32 == findWhat[k]) {
							k++;
							going = true;
						}
						else {
							going = false;
						}
					}
					else {
						if (one == findWhat[k]) {
							k++;
							going = true;
						}
						else {
							going = false;
						}
					}
				}
				else {
					going = false;
				}
			}
			else {
				if (dynamic_cast<MultiByteCharacter*>(character)) {
					other[0] = character->GetCharacters()[0];
					other[1] = character->GetCharacters()[1];
					if (other[0] != findWhat[k] || other[1] != findWhat[k + 1]) {
						going = false;
					}
					else {
						k += 2;
						going = true;
					}
				}
				else {
					going = false;
				}
			}
			if (going == false) {
				if (*row < i) {
					i = *row;
					glyph = this->GetAt(i - 1);
				}
				j = *start;
				k = 0;
			}
			j++;
		}
		i++;
	}
	if (findWhat[k] == '\0') {
		*end = j;
		*found = true;
		if (dynamic_cast<DummyRow*>(glyph)) {
			rowNumber = this->GetCloseRow(i - 1);
			index = rowNumber;
			while (index < *row) {
				glyph = this->GetAt(index - 1);
				(*start) += glyph->GetLength();
				(*end) += glyph->GetLength();
				index++;
			}
			while (index < i - 1) {
				glyph = this->GetAt(index - 1);
				(*end) += glyph->GetLength();
				index++;
			}
		}
		*row = this->GetRowNumber(i - 1);
	}
	else {
		*row = -1;
		*start = -1;
		*end = -1;
		*found = false;
	}
}

void Paper::FindNextMatchWholeWord(char* findWhat, bool* found, Long* row, Long* start, Long* end) {
	char one;
	char other[2];
	bool going = false;
	bool match = false;
	Long i = this->current;
	Long j;
	Long k = 0;
	Long index;
	Long rowNumber;
	Glyph* glyph = 0;
	Glyph* character;
	Glyph* line;

	while (i <= this->length && findWhat[k] != '\0') {
		glyph = this->GetAt(i - 1);
		if (!(dynamic_cast<DummyRow*>(glyph))) {
			k = 0;
		}
		if (i > this->current) {
			j = 0;
		}
		else {
			j = glyph->GetCurrent();
		}
		while (j < glyph->GetLength() && findWhat[k] != '\0') {
			if (k == 0) {
				*row = i;
				*start = j;
			}
			character = glyph->GetAt(j);
			if (findWhat[k] >= 9 && findWhat[k] <= 126) {
				if (dynamic_cast<SingleByteCharacter*>(character)) {
					one = character->GetCharacter();
					if (one >= 65 && one <= 90) {
						if (one == findWhat[k] || one + 32 == findWhat[k]) {
							k++;
							going = true;
						}
						else {
							going = false;
						}
					}
					else if (one >= 97 && one <= 122) {
						if (one == findWhat[k] || one - 32 == findWhat[k]) {
							k++;
							going = true;
						}
						else {
							going = false;
						}
					}
					else {
						if (one == findWhat[k]) {
							k++;
							going = true;
						}
						else {
							going = false;
						}
					}
				}
				else {
					going = false;
				}
			}
			else {
				if (dynamic_cast<MultiByteCharacter*>(character)) {
					other[0] = character->GetCharacters()[0];
					other[1] = character->GetCharacters()[1];
					if (other[0] != findWhat[k] || other[1] != findWhat[k + 1]) {
						going = false;
					}
					else {
						k += 2;
						going = true;
					}
				}
				else {
					going = false;
				}
			}
			if (going == false) {
				if (*row < i) {
					i = *row;
					glyph = this->GetAt(i - 1);
				}
				j = *start;
				k = 0;
			}
			else if (going && findWhat[k] == '\0') {
				if (*start == 0 && !(dynamic_cast<DummyRow*>(this->GetAt(*row - 1)))) {
					match = true;
				}
				else if (*start == 0 && dynamic_cast<DummyRow*>(this->GetAt(*row - 1))) {
					line = this->GetAt(*row - 2);
					character = line->GetAt(line->GetLength() - 1);
					if (character->GetContents()[0] == ' ') {
						match = true;
					}
				}
				else {
					line = this->GetAt(*row - 1);
					character = line->GetAt(*start - 1);
					if (character->GetContents()[0] == ' ') {
						match = true;
					}
				}
				if (match) {
					match = false;
					if (j + 1 < glyph->GetLength()) {
						character = glyph->GetAt(j + 1);
						if (character->GetContents()[0] == ' ') {
							match = true;
						}
					}
					else {
						if (i == this->length) {
							match = true;
						}
						else {
							line = this->GetAt(i);
							if (!(dynamic_cast<DummyRow*>(line))) {
								match = true;
							}
							else {
								character = line->GetAt(0);
								if (character->GetContents()[0] == ' ') {
									match = true;
								}
							}
						}
					}
				}
				if (match == false) {
					going = false;
					k = 0;
				}
			}
			j++;
		}
		i++;
	}
	if (findWhat[k] == '\0') {
		*end = j;
		*found = true;
		if (dynamic_cast<DummyRow*>(glyph)) {
			rowNumber = this->GetCloseRow(i - 1);
			index = rowNumber;
			while (index < *row) {
				glyph = this->GetAt(index - 1);
				(*start) += glyph->GetLength();
				(*end) += glyph->GetLength();
				index++;
			}
			while (index < i - 1) {
				glyph = this->GetAt(index - 1);
				(*end) += glyph->GetLength();
				index++;
			}
		}
		*row = this->GetRowNumber(i - 1);
	}
	else {
		*row = -1;
		*start = -1;
		*end = -1;
		*found = false;
	}
}

void Paper::FindNextMatchCase(char* findWhat, bool* found, Long* row, Long* start, Long* end) {
	bool going = false;
	char one;
	char other[2];
	Long i = this->current;
	Long j;
	Long k = 0;
	Long index;
	Long rowNumber;
	Glyph* glyph = 0;
	Glyph* character;

	while (i <= this->length && findWhat[k] != '\0') {
		glyph = this->GetAt(i - 1);
		if (!(dynamic_cast<DummyRow*>(glyph))) {
			k = 0;
		}
		if (i > this->current) {
			j = 0;
		}
		else {
			j = glyph->GetCurrent();
		}
		while (j < glyph->GetLength() && findWhat[k] != '\0') {
			if (k == 0) {
				*row = i;
				*start = j;
			}
			character = glyph->GetAt(j);
			if (findWhat[k] >= 9 && findWhat[k] <= 126) {
				if (dynamic_cast<SingleByteCharacter*>(character)) {
					one = character->GetCharacter();
					if (one == findWhat[k]) {
						k++;
						going = true;
					}
					else {
						going = false;
					}
				}
				else {
					going = false;
				}
			}
			else {
				if (dynamic_cast<MultiByteCharacter*>(character)) {
					other[0] = character->GetCharacters()[0];
					other[1] = character->GetCharacters()[1];
					if (other[0] != findWhat[k] || other[1] != findWhat[k + 1]) {
						going = false;
					}
					else {
						k += 2;
						going = true;
					}
				}
				else {
					going = false;
				}
			}
			if (going == false) {
				if (*row < i) {
					i = *row;
					glyph = this->GetAt(i - 1);
				}
				j = *start;
				k = 0;
			}
			j++;
		}
		i++;
	}
	if (findWhat[k] == '\0') {
		*end = j;
		*found = true;
		if (dynamic_cast<DummyRow*>(glyph)) {
			rowNumber = this->GetCloseRow(i - 1);
			index = rowNumber;
			while (index < *row) {
				glyph = this->GetAt(index - 1);
				(*start) += glyph->GetLength();
				(*end) += glyph->GetLength();
				index++;
			}
			while (index < i - 1) {
				glyph = this->GetAt(index - 1);
				(*end) += glyph->GetLength();
				index++;
			}
		}
		*row = this->GetRowNumber(i - 1);
	}
	else {
		*row = -1;
		*start = -1;
		*end = -1;
		*found = false;
	}
}

void Paper::FindNextMatchWholeWordAndCase(char* findWhat, bool* found, Long* row, Long* start, Long* end) {
	char one;
	char other[2];
	bool going = false;
	bool match = false;
	Long i = this->current;
	Long j;
	Long k = 0;
	Long index;
	Long rowNumber;
	Glyph* glyph = 0;
	Glyph* character;
	Glyph* line;

	while (i <= this->length && findWhat[k] != '\0') {
		glyph = this->GetAt(i - 1);
		if (!(dynamic_cast<DummyRow*>(glyph))) {
			k = 0;
		}
		if (i > this->current) {
			j = 0;
		}
		else {
			j = glyph->GetCurrent();
		}
		while (j < glyph->GetLength() && findWhat[k] != '\0') {
			if (k == 0) {
				*row = i;
				*start = j;
			}
			character = glyph->GetAt(j);
			if (findWhat[k] >= 9 && findWhat[k] <= 126) {
				if (dynamic_cast<SingleByteCharacter*>(character)) {
					one = character->GetCharacter();
					if (one == findWhat[k]) {
						k++;
						going = true;
					}
					else {
						going = false;
					}
				}
				else {
					going = false;
				}
			}
			else {
				if (dynamic_cast<MultiByteCharacter*>(character)) {
					other[0] = character->GetCharacters()[0];
					other[1] = character->GetCharacters()[1];
					if (other[0] != findWhat[k] || other[1] != findWhat[k + 1]) {
						going = false;
					}
					else {
						k += 2;
						going = true;
					}
				}
				else {
					going = false;
				}
			}
			if (going == false) {
				if (*row < i) {
					i = *row;
					glyph = this->GetAt(i - 1);
				}
				j = *start;
				k = 0;
			}
			else if (going && findWhat[k] == '\0') {
				if (*start == 0 && !(dynamic_cast<DummyRow*>(this->GetAt(*row - 1)))) {
					match = true;
				}
				else if (*start == 0 && dynamic_cast<DummyRow*>(this->GetAt(*row - 1))) {
					line = this->GetAt(*row - 2);
					character = line->GetAt(line->GetLength() - 1);
					if (character->GetContents()[0] == ' ') {
						match = true;
					}
				}
				else {
					line = this->GetAt(*row - 1);
					character = line->GetAt(*start - 1);
					if (character->GetContents()[0] == ' ') {
						match = true;
					}
				}
				if (match) {
					match = false;
					if (j + 1 < glyph->GetLength()) {
						character = glyph->GetAt(j + 1);
						if (character->GetContents()[0] == ' ') {
							match = true;
						}
					}
					else {
						if (i == this->length) {
							match = true;
						}
						else {
							line = this->GetAt(i);
							if (!(dynamic_cast<DummyRow*>(line))) {
								match = true;
							}
							else {
								character = line->GetAt(0);
								if (character->GetContents()[0] == ' ') {
									match = true;
								}
							}
						}
					}
				}
				if (match == false) {
					going = false;
					k = 0;
				}
			}
			j++;
		}
		i++;
	}
	if (findWhat[k] == '\0') {
		*end = j;
		*found = true;
		if (dynamic_cast<DummyRow*>(glyph)) {
			rowNumber = this->GetCloseRow(i - 1);
			index = rowNumber;
			while (index < *row) {
				glyph = this->GetAt(index - 1);
				(*start) += glyph->GetLength();
				(*end) += glyph->GetLength();
				index++;
			}
			while (index < i - 1) {
				glyph = this->GetAt(index - 1);
				(*end) += glyph->GetLength();
				index++;
			}
		}
		*row = this->GetRowNumber(i - 1);
	}
	else {
		*row = -1;
		*start = -1;
		*end = -1;
		*found = false;
	}
}

void Paper::FindPrevious(char* findWhat, bool* found, Long* row, Long* start, Long* end) {
	bool going = false;
	char one;
	char other[2];
	Long i = this->current;
	Long j;
	Long k = strlen(findWhat) - 1;
	Long index;
	Long rowNumber;
	Glyph* glyph = 0;
	Glyph* character;

	while (i >= 1 && k >= 0) {
		glyph = this->GetAt(i - 1);
		if (i < this->length && !(dynamic_cast<DummyRow*>(this->GetAt(i)))) {
			k = strlen(findWhat) - 1;
		}
		if (i < this->current) {
			j = glyph->GetLength() - 1;
		}
		else {
			j = glyph->GetCurrent() - 1;
		}
		while (j >= 0 && k >= 0) {
			if (k == strlen(findWhat) - 1) {
				*row = i;
				*end = j;
			}
			character = glyph->GetAt(j);
			if (findWhat[k] >= 9 && findWhat[k] <= 126) {
				if (dynamic_cast<SingleByteCharacter*>(character)) {
					one = character->GetCharacter();
					if (one >= 65 && one <= 90) {
						if (one == findWhat[k] || one + 32 == findWhat[k]) {
							k--;
							going = true;
						}
						else {
							going = false;
						}
					}
					else if (one >= 97 && one <= 122) {
						if (one == findWhat[k] || one - 32 == findWhat[k]) {
							k--;
							going = true;
						}
						else {
							going = false;
						}
					}
					else {
						if (one == findWhat[k]) {
							k--;
							going = true;
						}
						else {
							going = false;
						}
					}
				}
				else {
					going = false;
				}
			}
			else {
				if (dynamic_cast<MultiByteCharacter*>(character)) {
					other[0] = character->GetCharacters()[0];
					other[1] = character->GetCharacters()[1];
					if (other[0] != findWhat[k - 1] || other[1] != findWhat[k]) {
						going = false;
					}
					else {
						k -= 2;
						going = true;
					}
				}
				else {
					going = false;
				}
			}
			if (going == false) {
				if (*row < i) {
					i = *row;
					glyph = this->GetAt(i - 1);
				}
				j = *end;
				k = strlen(findWhat) - 1;
			}
			j--;
		}
		i--;
	}
	if (k < 0) {
		*start = j + 1;
		(*end)++;
		*found = true;
		rowNumber = this->GetCloseRow(i + 1);
		index = rowNumber;
		while (index < i + 1) {
			glyph = this->GetAt(index - 1);
			(*start) += glyph->GetLength();
			(*end) += glyph->GetLength();
			index++;
		}
		while (index < *row) {
			glyph = this->GetAt(index - 1);
			(*end) += glyph->GetLength();
			index++;
		}
		*row = this->GetRowNumber(i + 1);
	}
	else {
		*row = -1;
		*start = -1;
		*end = -1;
		*found = false;
	}
}

void Paper::FindPreviousMatchWholeWord(char* findWhat, bool* found, Long* row, Long* start, Long* end){
	char one;
	char other[2];
	bool going = false;
	bool match = false;
	Long i = this->current;
	Long j;
	Long k = strlen(findWhat) - 1;
	Long index;
	Long rowNumber;
	Glyph* line;
	Glyph* glyph = 0;
	Glyph* character;

	while (i >= 1 && k >= 0) {
		glyph = this->GetAt(i - 1);
		if (i < this->length && !(dynamic_cast<DummyRow*>(this->GetAt(i)))) {
			k = strlen(findWhat) - 1;
		}
		if (i < this->current) {
			j = glyph->GetLength() - 1;
		}
		else {
			j = glyph->GetCurrent() - 1;
		}
		while (j >= 0 && k >= 0) {
			if (k == strlen(findWhat) - 1) {
				*row = i;
				*end = j;
			}
			character = glyph->GetAt(j);
			if (findWhat[k] >= 9 && findWhat[k] <= 126) {
				if (dynamic_cast<SingleByteCharacter*>(character)) {
					one = character->GetCharacter();
					if (one >= 65 && one <= 90) {
						if (one == findWhat[k] || one + 32 == findWhat[k]) {
							k--;
							going = true;
						}
						else {
							going = false;
						}
					}
					else if (one >= 97 && one <= 122) {
						if (one == findWhat[k] || one - 32 == findWhat[k]) {
							k--;
							going = true;
						}
						else {
							going = false;
						}
					}
					else {
						if (one == findWhat[k]) {
							k--;
							going = true;
						}
						else {
							going = false;
						}
					}
				}
				else {
					going = false;
				}
			}
			else {
				if (dynamic_cast<MultiByteCharacter*>(character)) {
					other[0] = character->GetCharacters()[0];
					other[1] = character->GetCharacters()[1];
					if (other[0] != findWhat[k - 1] || other[1] != findWhat[k]) {
						going = false;
					}
					else {
						k -= 2;
						going = true;
					}
				}
				else {
					going = false;
				}
			}
			if (going == false) {
				if (*row < i) {
					i = *row;
					glyph = this->GetAt(i - 1);
				}
				j = *end;
				k = strlen(findWhat) - 1;
			}
			else if (going && k < 0) {
				if (j == 0 && !(dynamic_cast<DummyRow*>(glyph))) {
					match = true;
				}
				else if (j == 0 && dynamic_cast<DummyRow*>(glyph)) {
					line = this->GetAt(i - 2);
					character = line->GetAt(line->GetLength() - 1);
					if (character->GetContents()[0] == ' ') {
						match = true;
					}
				}
				else {
					character = glyph->GetAt(j - 1);
					if (character->GetContents()[0] == ' ') {
						match = true;
					}
				}
				if (match) {
					match = false;
					line = this->GetAt(*row - 1);
					if (*end + 1 < line->GetLength()) {
						character = line->GetAt(*end + 1);
						if (character->GetContents()[0] == ' ') {
							match = true;
						}
					}
					else {
						if (*row == this->length) {
							match = true;
						}
						else {
							line = this->GetAt(*row);
							if (!(dynamic_cast<DummyRow*>(line))) {
								match = true;
							}
							else {
								character = line->GetAt(0);
								if (character->GetContents()[0] == ' ') {
									match = true;
								}
							}
						}
					}
				}
				if (match == false) {
					going = false;
					k = 0;
				}
			}
			j--;
		}
		i--;
	}
	if (k < 0) {
		*start = j + 1;
		(*end)++;
		*found = true;
		rowNumber = this->GetCloseRow(i + 1);
		index = rowNumber;
		while (index < i + 1) {
			glyph = this->GetAt(index - 1);
			(*start) += glyph->GetLength();
			(*end) += glyph->GetLength();
			index++;
		}
		while (index < *row) {
			glyph = this->GetAt(index - 1);
			(*end) += glyph->GetLength();
			index++;
		}
		*row = this->GetRowNumber(i + 1);
	}
	else {
		*row = -1;
		*start = -1;
		*end = -1;
		*found = false;
	}
}

void Paper::FindPreviousMatchCase(char* findWhat, bool* found, Long* row, Long* start, Long* end){
	bool going = false;
	char one;
	char other[2];
	Long i = this->current;
	Long j;
	Long k = strlen(findWhat) - 1;
	Long index;
	Long rowNumber;
	Glyph* glyph = 0;
	Glyph* character;

	while (i >= 1 && k >= 0) {
		glyph = this->GetAt(i - 1);
		if (i < this->length && !(dynamic_cast<DummyRow*>(this->GetAt(i)))) {
			k = strlen(findWhat) - 1;
		}
		if (i < this->current) {
			j = glyph->GetLength() - 1;
		}
		else {
			j = glyph->GetCurrent() - 1;
		}
		while (j >= 0 && k >= 0) {
			if (k == strlen(findWhat) - 1) {
				*row = i;
				*end = j;
			}
			character = glyph->GetAt(j);
			if (findWhat[k] >= 9 && findWhat[k] <= 126) {
				if (dynamic_cast<SingleByteCharacter*>(character)) {
					one = character->GetCharacter();
					if (one == findWhat[k]) {
						k--;
						going = true;
					}
					else {
						going = false;
					}
				}
				else {
					going = false;
				}
			}
			else {
				if (dynamic_cast<MultiByteCharacter*>(character)) {
					other[0] = character->GetCharacters()[0];
					other[1] = character->GetCharacters()[1];
					if (other[0] != findWhat[k - 1] || other[1] != findWhat[k]) {
						going = false;
					}
					else {
						k -= 2;
						going = true;
					}
				}
				else {
					going = false;
				}
			}
			if (going == false) {
				if (*row < i) {
					i = *row;
					glyph = this->GetAt(i - 1);
				}
				j = *end;
				k = strlen(findWhat) - 1;
			}
			j--;
		}
		i--;
	}
	if (k < 0) {
		*start = j + 1;
		(*end)++;
		*found = true;
		rowNumber = this->GetCloseRow(i + 1);
		index = rowNumber;
		while (index < i + 1) {
			glyph = this->GetAt(index - 1);
			(*start) += glyph->GetLength();
			(*end) += glyph->GetLength();
			index++;
		}
		while (index < *row) {
			glyph = this->GetAt(index - 1);
			(*end) += glyph->GetLength();
			index++;
		}
		*row = this->GetRowNumber(i + 1);
	}
	else {
		*row = -1;
		*start = -1;
		*end = -1;
		*found = false;
	}
}

void Paper::FindPreviousMatchWholeWordAndCase(char* findWhat, bool* found, Long* row, Long* start, Long* end){
	char one;
	char other[2];
	bool going = false;
	bool match = false;
	Long i = this->current;
	Long j;
	Long k = strlen(findWhat) - 1;
	Long index;
	Long rowNumber;
	Glyph* line;
	Glyph* glyph = 0;
	Glyph* character;

	while (i >= 1 && k >= 0) {
		glyph = this->GetAt(i - 1);
		if (i < this->length && !(dynamic_cast<DummyRow*>(this->GetAt(i)))) {
			k = strlen(findWhat) - 1;
		}
		if (i < this->current) {
			j = glyph->GetLength() - 1;
		}
		else {
			j = glyph->GetCurrent() - 1;
		}
		while (j >= 0 && k >= 0) {
			if (k == strlen(findWhat) - 1) {
				*row = i;
				*end = j;
			}
			character = glyph->GetAt(j);
			if (findWhat[k] >= 9 && findWhat[k] <= 126) {
				if (dynamic_cast<SingleByteCharacter*>(character)) {
					one = character->GetCharacter();
					if (one == findWhat[k]) {
						k--;
						going = true;
					}
					else {
						going = false;
					}
				}
				else {
					going = false;
				}
			}
			else {
				if (dynamic_cast<MultiByteCharacter*>(character)) {
					other[0] = character->GetCharacters()[0];
					other[1] = character->GetCharacters()[1];
					if (other[0] != findWhat[k - 1] || other[1] != findWhat[k]) {
						going = false;
					}
					else {
						k -= 2;
						going = true;
					}
				}
				else {
					going = false;
				}
			}
			if (going == false) {
				if (*row < i) {
					i = *row;
					glyph = this->GetAt(i - 1);
				}
				j = *end;
				k = strlen(findWhat) - 1;
			}
			else if (going && k < 0) {
				if (j == 0 && !(dynamic_cast<DummyRow*>(glyph))) {
					match = true;
				}
				else if (j == 0 && dynamic_cast<DummyRow*>(glyph)) {
					line = this->GetAt(i - 2);
					character = line->GetAt(line->GetLength() - 1);
					if (character->GetContents()[0] == ' ') {
						match = true;
					}
				}
				else {
					character = glyph->GetAt(j - 1);
					if (character->GetContents()[0] == ' ') {
						match = true;
					}
				}
				if (match) {
					match = false;
					line = this->GetAt(*row - 1);
					if (*end + 1 < line->GetLength()) {
						character = line->GetAt(*end + 1);
						if (character->GetContents()[0] == ' ') {
							match = true;
						}
					}
					else {
						if (*row == this->length) {
							match = true;
						}
						else {
							line = this->GetAt(*row);
							if (!(dynamic_cast<DummyRow*>(line))) {
								match = true;
							}
							else {
								character = line->GetAt(0);
								if (character->GetContents()[0] == ' ') {
									match = true;
								}
							}
						}
					}
				}
				if (match == false) {
					going = false;
					k = 0;
				}
			}
			j--;
		}
		i--;
	}
	if (k < 0) {
		*start = j + 1;
		(*end)++;
		*found = true;
		rowNumber = this->GetCloseRow(i + 1);
		index = rowNumber;
		while (index < i + 1) {
			glyph = this->GetAt(index - 1);
			(*start) += glyph->GetLength();
			(*end) += glyph->GetLength();
			index++;
		}
		while (index < *row) {
			glyph = this->GetAt(index - 1);
			(*end) += glyph->GetLength();
			index++;
		}
		*row = this->GetRowNumber(i + 1);
	}
	else {
		*row = -1;
		*start = -1;
		*end = -1;
		*found = false;
	}
}

void Paper::ConvertChar(Long* index, Long* length, Long* rowLength, char* buffer) {
	Glyph* row;

	*length = 0;
	memset(buffer, 127, sizeof(char) * (MAX - 2));
	row = this->GetAt(*index);
	*rowLength = row->GetRowLength();
	row->ConvertChar(buffer, length);
	(*index)++;
	while (*index < this->length && dynamic_cast<DummyRow*>(this->GetAt(*index))) {
		row = this->GetAt(*index);
		*rowLength += row->GetRowLength();
		row->ConvertChar(buffer, length);
		(*index)++;
	}
}

void Paper::ConvertChar(Long* index, Long* length, Long* rowLength, char* buffer, CharacterMetrics* characterMetrics) {
	Glyph* row;

	*rowLength = 0;
	*length = 0;
	memset(buffer, 127, sizeof(char) * (MAX - 2));
	row = this->GetAt(*index);
	row->ConvertChar(buffer, length, characterMetrics);
	*rowLength += dynamic_cast<Row*>(row)->rowLength;
	(*index)++;
	while (*index < this->length && dynamic_cast<DummyRow*>(this->GetAt(*index))) {
		row = this->GetAt(*index);
		dynamic_cast<Row*>(row)->rowLength = 0;
		row->ConvertChar(buffer, length, characterMetrics);
		*rowLength += dynamic_cast<Row*>(row)->rowLength;
		(*index)++;
	}
}

Paper& Paper::operator=(const Paper& source) {
	Composite::operator=(source);

	return *this;
}
