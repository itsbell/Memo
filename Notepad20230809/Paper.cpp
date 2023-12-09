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
클래스명: Paper
기    능: Paper -▷ Composite -▷ Glyph / Paper 종이에 대한 클래스(Concrete Class)
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
함수명칭: Clone
기    능: 현재의 객체와 똑같은 객체를 생성하다. (관련 디자인패턴: Prototype)
입    력: 없음
출    력: Glyph Link
*/
Glyph* Paper::Clone() {
	return new Paper(*this);
}

/*
함수명칭: GetContents
기    능: 해당 객체의 내용을 구하다.
입    력: 없음
출    력: string
*/
string Paper::GetContents() {
	string contents("");
	Long i = 0;

	while (i < this->length) {
		contents += this->glyphs[i]->GetContents();

		//마지막줄이 아니면 개행을 하다.
		if (i != this->length - 1) {
			contents += "\n";
		}
		
		i++;
	}

	return contents;
}

/*
함수명칭: Accept
기    능: 입력되는 GlyphVisitor의 Concrete클래스에 따라 해당 클래스의 Visit함수를 호출한다.
입    력: GlyphVisitor Link
출    력: 없음
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
	
	//1. 적을 위치를 기억하다.
	current = this->current;
	row = this->GetAt(current - 1);
	column = row->GetCurrent();
	
	//2. 줄 수를 구하다.
	count = this->length;

	//3. 줄 수만큼 반복하다.
	while (i < count) {
		row = this->GetAt(0);
		if (row->GetLength() == 0) {
			glyph = row->Clone();
			this->Add(glyph);
		}
		else {
			//3.1. 문자 수만큼 반복하다.
			j = 0;
			while (j < row->GetLength()) {
				//3.2. 줄에서 자동줄바꿈하여 종이에 추가하다.
				row->Wrap(width, characterMetrics, &j, &glyph);
				this->Add(glyph);
			}
		}	
		//3.2. 자동줄바꿈한 줄을 지우다.
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
	//4. 종이에서 위치를 이동하다.
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
	
	//1. 현재 줄 위치를 저장하다.
	currentRow = this->GetRowNumber(this->current);
	column = this->GetColumnNumber();

	//3. 줄 수만큼 반복하다.
	while (i < this->length) {
		current = this->GetAt(i);
		//3.1. 마지막 줄이 아니고 DummyRow인 동안 반복하다.
		while(i + 1 < this->length && dynamic_cast<DummyRow*>(this->GetAt(i + 1))){
			//3.1.1. 문자 수만큼 반복하다.
			row = this->GetAt(i + 1);
			j = 0;
			while (j < row->GetLength()) {
				character = row->GetAt(j);
				current->Add(character->Clone(), characterMetrics);
				j++;
			}
			i++;
		}
		//3.2. 마지막 줄이거나 Row이면
		if (i + 1 == this->length){
			i++;
		}
		else if (!(dynamic_cast<DummyRow*>(this->GetAt(i + 1)))) {
			i++;
		}
	}
	//4. DummyRow를 모두 지우다.
	i = this->length - 1;
	while (i >= 0) {
		if (dynamic_cast<DummyRow*>(this->GetAt(i))) {
			this->Remove(i);
		}
		i--;
	}
	//5. 현재 적을 위치를 이동하다.
	this->current = currentRow;
	row = this->GetAt(currentRow - 1);
	row->Move(column);
}

/*
함수명칭: RowWrap
기    능: 줄 단위로 자동줄바꿈을 하다.
입    력: 클라이언트 가로크기, CharacterMetrics Link
출    력: 없음
*/
void Paper::RowWrap(Long width, CharacterMetrics* characterMetrics) {
	Long i = 0;
	Glyph* character;
	Glyph* row;
	Glyph* current = this->GetAt(this->current - 1); // 현재 줄을 구하다.
	GlyphFactory glyphFactory;
	Long currentRow = this->current; // 종이에서 현재위치를 저장하다.
	Long currentColumn = current->GetCurrent(); // 줄에서 현재위치를 저장하다.
	Long rowWidth = 0;

	//1. Row를 생성하다.
	row = glyphFactory.Create((char*)"\r\n");

	//2. 현재 줄이 선택상태이면 선택하다.
	if (current->GetIsSelected() == true) {
		row->Select();
	}

	//3. 종이에서 추가하다.
	this->Add(this->current, row);

	//4. 현재 줄의 문자 수만큼 반복하다.
	while (i < current->GetLength()) {

		//4.1. 문자를 구하다.
		character = current->GetAt(i);

		//4.2. 줄길이와 문자 폭의 합이 클라이언트 가로길이보다 작거나 같으면
		if (rowWidth + character->GetWidth(characterMetrics) <= width) {

			//4.2.1. 줄에서 문자를 추가하다.
			row->Add(character->Clone(), characterMetrics);

			//4.2.2. 줄길이를 구하다.
			rowWidth += character->GetWidth(characterMetrics);
		}

		//4.3. 줄길이와 문자 폭의 합이 클라이언트 가로길이 보다 크면
		else {

			//4.3.1. DummyRow를 만들다.
			row = glyphFactory.Create((char*)"\r");

			//4.3.2. 종이에서 추가하다.
			this->Add(this->current, row);

			//4.3.3. 줄 길이를 원위치하다.
			rowWidth = 0;

			//4.4.4. 줄에서 문자를 추가하다.
			row->Add(character->Clone(), characterMetrics);

			//4.4.5. 줄 길이를 구하다.
			rowWidth += character->GetWidth(characterMetrics);
		}
		i++;
	}

	//5. 자동줄바꿈 완료된 줄을 지우다.
	this->Remove(currentRow - 1);
	if (currentRow > 1) {
		this->current++;
	}
	
	current = this->GetAt(this->current -1);
	

	//6. 자동줄바꿈 전 현재위치를 자동줄바꿈 후 동일 위치로 이동하다.
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
함수명칭: RowUnWrap
기    능: 줄 단위로 자동줄바꿈을 하다.
입    력: 클라이언트 가로크기, CharacterMetrics Link
출    력: 없음
*/
void Paper::RowUnWrap(CharacterMetrics* characterMetrics) {
	Long index;
	Glyph* nextRow;
	Glyph* character;
	Glyph* current = this->GetAt(this->current - 1); // 현재 줄을 구하다.
	Long column = current->GetCurrent(); // 줄에서 현재 칸을 구하다.
	
	//1. 현재 줄이 DummyRow인 동안 반복하다.
	while (dynamic_cast<DummyRow*>(current)) {
		//1.1. 종이에서 이전으로 이동하다.
		index = this->Previous();
		//1.2. 현재 줄을 구하다.
		current = this->GetAt(index - 1);
		//1.3. 칸을 구하다.
		column += current->GetLength();
	}

	//2. 마지막 줄이 아니고 다음줄이 DummyRow인 동안 반복하다.
	while (this->current < this->length && dynamic_cast<DummyRow*>(this->GetAt(this->current))) {
		//2.1. index를 원위치하다.
		index = 0;
		//2.2. 다음 줄을 구하다.
		nextRow = this->GetAt(this->current);
		//2.3. 다음 줄 문자수만큼 반복하다.
		while (index < nextRow->GetLength()) {
			//2.3.1. 문자를 구하다.
			character = nextRow->GetAt(index);
			//2.3.2. 줄에 문자를 추가하다.
			current->Add(character->Clone(), characterMetrics);
			//2.3.3. index를 세다.
			index++;
		}
		//2.4. 다 옮겨 적은 줄을 지우다.
		this->Remove(this->current);
	}
	//3. 현재줄에서 이동하다.
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

	//1. 줄이 Row이면서 선택상태이거나 DummyRow이면서 첫번째 문자가 선택상태이면 i를 세다.
	while ((!(dynamic_cast<DummyRow*>(this->GetAt(i - 1))) && this->GetAt(i - 1)->GetIsSelected() == true) ||
		(dynamic_cast<DummyRow*>(this->GetAt(i - 1)) && this->GetAt(i - 2)->GetAt(this->GetAt(i - 2)->GetLength() - 1)->GetIsSelected() == true)) {
		i--;
	}

	//2. 줄을 구하다.
	row = this->GetAt(i - 1);

	//3. 줄에 문자가 있으면
	if (row->GetLength() > 0) {
		
		//3.1. 마지막칸이 아니고 문자가 미선택 상태인 동안 반복하다.
		while (j < row->GetLength() && row->GetAt(j)->GetIsSelected() == false) {
			j++;
		}
#if 0
		//3.2. 마지막칸이면
		if (j == row->GetLength()) {

			//3.2.1. 현재줄이 DummyRow이면 다음줄로 이동하다.
			row = this->GetAt(i);
			if (dynamic_cast<DummyRow*>(row)) {
				i++;
				j = 0;
			}
		}
#endif
	}

	//4. i가 줄 수보다 작거나 같고 isSelecting가 TRUE인 동안 반복하다.
	startRow = i;
	while (i <= this->length && isSelecting == true) {
		
		//4.1. 현재줄이 Row이고 선택상태이면
		if (!(dynamic_cast<DummyRow*>(this->GetAt(i - 1))) && this->GetAt(i - 1)->GetIsSelected() == true) {

			//4.1.1. isSelecting를 고치다.
			isSelecting = false;

			//4.1.2. compare를 고치다.
			compare = false;
		}

#if 0
		//4.2. i가 시작줄이 아니고 Row이면서 미선택상태이면 isSelecting를 고치다.
		else if (i != startRow && !(dynamic_cast<DummyRow*>(this->GetAt(i - 1)) && this->GetAt(i - 1)->GetIsSelected() == false)) {
			isSelecting = false;
		}

		//4.3. i가 시작줄이 아니고 DummyRow이면서 첫번째 문자가 미선택 상태이면 isSelecting를 고치다.
		else if (i != startRow && dynamic_cast<DummyRow*>(this->GetAt(i - 1)) && this->GetAt(i - 1)->GetAt(0)->GetIsSelected() == false) {
			isSelecting = false;
		}
#endif
		if (i > startRow) {
			j = 0;
		}
		//4.4. j가 문자수보다 작고 isSelecting가 TRUE인 동안 반복하다.
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

	//1. 현재 줄부터 위 방향으로 미선택 줄을 찾다.
	while (!dynamic_cast<DummyRow*>(this->GetAt(this->current - 1)) && this->GetAt(this->current - 1)->GetIsSelected() == true ||
			dynamic_cast<DummyRow*>(this->GetAt(this->current - 1)) &&
			this->GetAt(this->current - 2)->GetAt(this->GetAt(this->current - 2)->GetLength()-1)->GetIsSelected() == true){
		this->Previous();
	}

	//2. 찾은 줄부터 마지막까지 그리고 copy Flag가 true인 동안 반복하다.
	i = this->current - 1;
	while (i < this->length && copy == true) {
		
		//2.1. 줄을 구하다.
		row = this->GetAt(i);
		//2.2. 줄이 Row이고 찾은 첫번째 줄이거나, 줄이 Row이고 선택상태이면
		if ((!(dynamic_cast<DummyRow*>(row)) && i == this->current - 1) ||
			(!(dynamic_cast<DummyRow*>(row)) && row->GetIsSelected() == true)) {

			//2.2.1. 찾은 첫번째 줄이 아니면 text에 개행문자를 추가하다.
			if (i > this->current - 1) {
				text += "\n";
			}
			//2.2.2. 줄에서 복사하다.
			contents = row->Copy();
		}

		//2.3. 줄이 Row이고 미선택 상태이면 복사를 끝내다.
		else if (!(dynamic_cast<DummyRow*>(row)) && row->GetIsSelected() == false) {
			contents = "";
			copy = false;
		}

		//2.4. 줄이 DummyRow이면
		else if (dynamic_cast<DummyRow*>(row)) {

			//2.4.1. 줄의 첫번째 문자가 선택상태이거나 찾은 첫번째 줄이면 줄에서 복사하다.
			if (row->GetAt(0)->GetIsSelected() == true || i == this->current - 1) {
				contents = row->Copy();
			}

			//2.4.2. 줄의 첫번째 문자가 선택상태가 아니면 복사를 끝내다.
			else {
				contents = "";
				copy = false;
			}
		}

		//2.5. text에 줄에서 복사한 내용을 추가하다.
		text += contents;

		//2.6. 줄을 세다.
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

	//1. 현재줄을 저장하다.
	currentRow = this->GetAt(this->current - 1);
	row = currentRow;

	//2. 입력된 문자가 끝이 아닌 동안 반복하다.
	while (text[i] != '\0') {

		//2.1. 개행문자이면
		if (text[i] == '\r') {
			//2.1.1. 종이에서 줄을 끼우다.
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

		//2.2. ASCII 코드이면
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

		//2.3. ASCII 코드가 아니면
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
