//Row.cpp
#include "Row.h"
#include "GlyphVisitor.h"
#include "GlyphFactory.h"
#include "CharacterMetrics.h"
#include "SingleByteCharacter.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Ŭ������: Row
��    ��: Row -�� Composite -�� Glyph / Row �ٿ� ���� Ŭ����(Concrete Class)
*/

Row::Row(Long capacity)
	:Composite(capacity) {
	this->isSelected = false;
	this->rowLength = 0;
}

Row::Row(const Row& source)
	: Composite(source) {
	this->isSelected = source.isSelected;
	this->rowLength = source.rowLength;
}

Row::~Row() {

}


Long Row::Add(Glyph* glyph, CharacterMetrics* characterMetrics) {
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
	this->rowLength += glyph->GetWidth(characterMetrics);

	return index;
}


Long Row::Add(Long index, Glyph* glyph, CharacterMetrics* characterMetrics) {

	index = this->glyphs.Insert(index, glyph);

	this->capacity++;
	this->length++;
	this->current = index + 1;
	this->rowLength += glyph->GetWidth(characterMetrics);

	return index;
}


Long Row::Remove(Long index, CharacterMetrics* characterMetrics) {

	this->current = index;
	this->rowLength -= this->glyphs[index]->GetWidth(characterMetrics);

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

/*
�Լ���Ī: Clone
��    ��: ������ ��ü�� �Ȱ��� ��ü�� �����ϴ�. (���� ����������: Prototype)
��    ��: ����
��    ��: Glyph Link
*/
Glyph* Row::Clone() {
	return new Row(*this);
}

/*
�Լ���Ī: GetContents
��    ��: �ش� ��ü�� ������ ���ϴ�.
��    ��: ����
��    ��: string
*/
string Row::GetContents() {
	string contents("");
	Long i = 0;

	while (i < this->length) {
		contents += this->glyphs[i]->GetContents();
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
void Row::Accept(GlyphVisitor* visitor) {
	visitor->Visit(this);
}

Long Row::First() {

	this->current = 0;

	return this->current;
}

Long Row::Last() {

	this->current = this->length;

	return this->current;
}

Long Row::Previous() {
	
	this->current--;

	if (this->current < 0) {
		this->current++;
	}

	return this->current;
}

Long Row::Next() {

	this->current++;

	if (this->current > this->length) {
		this->current--;
	}

	return this->current;
}

Long Row::Move(Long destination, CharacterMetrics* characterMetrics) {
	Long i = 0;
	Long x = 0;
	Long one;
	Long other;
	Long previous;
	Glyph* character = 0;

	while (i < this->length && x < destination) {
		character = this->GetAt(i);
		x += character->GetWidth(characterMetrics);
		i++;
	}
	if (i > 0 && i <= this->length && destination != x) {
		previous = x - character->GetWidth(characterMetrics);
		one = destination - previous;
		other = x - destination;

		if (one < other) {
			i--;
			x = previous;
		}
	}
	this->current = i;

	return this->current;
}

Long Row::PreviousWord() {

	//1. ������ġ�� ó���� �ƴϰ� SPACE�� �ƴѵ��� �ݺ��ϴ�.
	while (this->current > 0 && this->GetAt(this->current - 1)->GetContents().compare(string(" ")) == 0) {

		//1.1. �������� �̵��ϴ�.
		this->current--;
	}

	//2. ������ġ�� ó���� �ƴϰ� �ѱ�/����/�����̸�
	if (this->current > 0 &&
		(!(this->GetAt(this->current - 1)->GetContents()[0] >= 0 && this->GetAt(this->current - 1)->GetContents()[0] < ASCII) ||
			this->GetAt(this->current - 1)->GetContents()[0] >= 48 && this->GetAt(this->current - 1)->GetContents()[0] <= 57 ||
			this->GetAt(this->current - 1)->GetContents()[0] >= 65 && this->GetAt(this->current - 1)->GetContents()[0] <= 90 ||
			this->GetAt(this->current - 1)->GetContents()[0] >= 97 && this->GetAt(this->current - 1)->GetContents()[0] <= 122)) {

		//2.1. ������ġ�� ó���� �ƴϰ� �ѱ�/����/������ ���� �ݺ��ϴ�.
		while (this->current > 0 &&
			(!(this->GetAt(this->current - 1)->GetContents()[0] >= 0 && this->GetAt(this->current - 1)->GetContents()[0] < ASCII) ||
				this->GetAt(this->current - 1)->GetContents()[0] >= 48 && this->GetAt(this->current - 1)->GetContents()[0] <= 57 ||
				this->GetAt(this->current - 1)->GetContents()[0] >= 65 && this->GetAt(this->current - 1)->GetContents()[0] <= 90 ||
				this->GetAt(this->current - 1)->GetContents()[0] >= 97 && this->GetAt(this->current - 1)->GetContents()[0] <= 122)) {

			//2.1.1. �������� �̵��ϴ�.
			this->current--;
		}
	}

	//3. ������ġ�� ó���� �ƴϰ� Ư�������̸�
	else if (this->current > 0 &&
			(this->GetAt(this->current - 1)->GetContents()[0] >= 33 && this->GetAt(this->current - 1)->GetContents()[0] <= 47 ||
			this->GetAt(this->current - 1)->GetContents()[0] >= 58 && this->GetAt(this->current - 1)->GetContents()[0] <= 64 ||
			this->GetAt(this->current - 1)->GetContents()[0] >= 91 && this->GetAt(this->current - 1)->GetContents()[0] <= 96 ||
			this->GetAt(this->current - 1)->GetContents()[0] >= 123 && this->GetAt(this->current - 1)->GetContents()[0] <= 126)) {

		//3.1. ������ġ�� ó���� �ƴϰ� Ư�������� ���� �ݺ��ϴ�.
		while (this->current > 0 &&
			(this->GetAt(this->current - 1)->GetContents()[0] >= 33 && this->GetAt(this->current - 1)->GetContents()[0] <= 47 ||
				this->GetAt(this->current - 1)->GetContents()[0] >= 58 && this->GetAt(this->current - 1)->GetContents()[0] <= 64 ||
				this->GetAt(this->current - 1)->GetContents()[0] >= 91 && this->GetAt(this->current - 1)->GetContents()[0] <= 96 ||
				this->GetAt(this->current - 1)->GetContents()[0] >= 123 && this->GetAt(this->current - 1)->GetContents()[0] <= 126)) {

			//3.1.1. �������� �̵��ϴ�.
			this->current--;
		}
	}

	//4. ������ġ�� ó���� �ƴϰ� TAB�̸� �������� �̵��ϴ�.
	else if (this->current > 0 && this->GetAt(this->current - 1)->GetContents()[0] == '\t') {
		this->current--;
	}

	return this->current;
}

Long Row::NextWord() {

	//1. ������ĭ�� �ƴϸ�
	if (this->current < this->length) {
		Glyph* glyph;
		string character;
		
		//1.1. ������ġ ���ڸ� ���ϴ�.
		glyph = this->GetAt(this->current);
		character = glyph->GetContents();

		//1.2. ���ڰ� �ѱ�/����/���� �̸�
		if (!(character[0] >= 0 && character[0] < ASCII) || character[0] >= 48 && character[0] <= 57 ||
			character[0] >= 65 && character[0] <= 90 || character[0] >= 97 && character[0] <= 122) {
			
			//1.2.1. ����ĭ���� �̵��ϴ�.
			this->current++;
			
			//1.2.2. ������ĭ�� �ƴϰ� ������ġ�� ���ڰ� �ѱ�/����/������ ���� �ݺ��ϴ�.
			while (this->current < this->length &&
				(!(this->GetAt(this->current)->GetContents()[0] >= 0 && this->GetAt(this->current)->GetContents()[0] < ASCII) ||
				this->GetAt(this->current)->GetContents()[0] >= 48 && this->GetAt(this->current)->GetContents()[0] <= 57 ||
				this->GetAt(this->current)->GetContents()[0] >= 65 && this->GetAt(this->current)->GetContents()[0] <= 90 ||
				this->GetAt(this->current)->GetContents()[0] >= 97 && this->GetAt(this->current)->GetContents()[0] <= 122)) {

				//1.2.2.1. ����ĭ���� �̵��ϴ�.
				this->current++;
			}
		}

		//1.2. ���ڰ� Ư������ �̸�
		else if (character[0] >= 33 && character[0] <= 47 || character[0] >= 58 && character[0] <= 64 ||
			character[0] >= 91 && character[0] <= 96 || character[0] >= 123 && character[0] <= 126) {

			//1.2.1. ����ĭ���� �̵��ϴ�.
			this->current++;

			//1.2.2. ������ĭ�� �ƴϰ� ������ġ�� ���ڰ� Ư�������� ���� �ݺ��ϴ�.
			while (this->current < this->length &&
				(this->GetAt(this->current)->GetContents()[0] >= 33 && this->GetAt(this->current)->GetContents()[0] <= 47 ||
				this->GetAt(this->current)->GetContents()[0] >= 58 && this->GetAt(this->current)->GetContents()[0] <= 64 ||
				this->GetAt(this->current)->GetContents()[0] >= 91 && this->GetAt(this->current)->GetContents()[0] <= 96 ||
				this->GetAt(this->current)->GetContents()[0] >= 123 && this->GetAt(this->current)->GetContents()[0] <= 126)) {

				//1.2.2.1. ����ĭ���� �̵��ϴ�.
				this->current++;
			}
		}

		//1.3. ���ڰ� TAB�̸� ����ĭ���� �̵��ϴ�.
		else if (character[0] == 9) {
			this->current++;
		}

		//1.4. ������ĭ�� �ƴϰ� ������ġ ���ڰ� SPACE�̸� ����ĭ���� �̵��ϴ�.
		while (this->current < this->length && this->GetAt(this->current)->GetContents()[0] == ' ') {
			this->current++;
		}
	}

	return this->current;
}

Long Row::GetWidth(CharacterMetrics* characterMetrics) {
	Glyph* glyph;
	Long width = 0;
	Long i = 0;

	while (i < this->length) {
		glyph = this->GetAt(i);
		width += glyph->GetWidth(characterMetrics);
		i++;
	}

	return width;
}

void Row::Wrap(int width, CharacterMetrics* characterMetrics, Long* index, Glyph** glyph) {
	Long rowWidth = 0;
	Long charWidth;
	Glyph* character;
	GlyphFactory glyphFactory;
	
	//1. ù��° ĭ�̸� Row�� �����.
	if (*index == 0) {
		*glyph = glyphFactory.Create((char*)"\r\n");
		if (this->isSelected == true) {
			(*glyph)->Select();
		}
		else {
			(*glyph)->UnSelect();
		}
	}

	//2. ù��° ĭ�� �ƴϸ� DummyRow�� �����.
	else {
		*glyph = glyphFactory.Create((char*)"\r");
	}
	
	//3. ���� ĭ ���� ���� ���ϴ�.
	character = this->GetAt(*index);
	charWidth = character->GetWidth(characterMetrics);

	//4. ������ĭ�� �ƴϰ� �� ���� Ŭ���̾�Ʈ ����ũ�⸦ ���� �ʴ� ���� �ݺ��ϴ�.
	while (*index < this->length && (*glyph)->GetRowLength() + charWidth <= width) {
		//4.1. �ٿ� ���ڸ� �߰��ϴ�.
		(*glyph)->Add(this->GetAt(*index)->Clone(), characterMetrics);
		//4.2. ������ĭ�� �ƴϸ�
		if (*index < this->length - 1) {
			//4.2.1. ���� ĭ ���� ���� ���ϴ�.
			character = this->GetAt((*index) + 1);
			charWidth = character->GetWidth(characterMetrics);
		}
		//4.3. ������ĭ�̸� ���� ���� 0���� �ϴ�.
		else {
			charWidth = 0;
		}
		(*index)++;
	}
}

void Row::Select() {
	this->isSelected = true;
}

void Row::SelectContents() {
	Long i = 0;
	Glyph* glyph;

	while (i < this->length) {
		glyph = this->GetAt(i);
		if (glyph->GetIsSelected() == false) {
			glyph->Select();
		}
		else {
			glyph->UnSelect();
		}
		i++;
	}
}

void Row::SelectContents(Long start, Long end) {
	Glyph* character;

	while (start < end) {
		character = this->GetAt(start);
		if (character->GetIsSelected() == false) {
			character->Select();
		}
		else {
			character->UnSelect();
		}
		start++;
	}
}

void Row::UnSelect() {
	this->isSelected = false;
}

string Row::Copy() {
	Long i = 0;
	string contents;
	string character;
	
	while (i < this->length && this->GetAt(i)->GetIsSelected() == false) {
		i++;
	}

	while (i < this->length && this->GetAt(i)->GetIsSelected() == true) {
		character = this->GetAt(i)->GetContents();
		contents += character;
		i++;
	}
	
	return contents;
}

void Row::ConvertChar(char* buffer, Long* length) {
	Long i = 0;
	Glyph* character;

	while (i < this->length) {
		character = this->GetAt(i);
		if (dynamic_cast<SingleByteCharacter*>(character)) {
			buffer[*length] = character->GetCharacter();
		}
		else {
			buffer[*length] = character->GetCharacters()[0];
			(*length)++;
			buffer[*length] = character->GetCharacters()[1];
		}
		(*length)++;
		i++;
	}
}

void Row::ConvertChar(char* buffer, Long* length, CharacterMetrics* characterMetrics) {
	Long i = 0;
	Glyph* character;

	this->rowLength = 0;
	while (i < this->length) {
		character = this->GetAt(i);
		if (dynamic_cast<SingleByteCharacter*>(character)) {
			buffer[*length] = character->GetCharacter();
			this->rowLength += characterMetrics->GetWidth(character->GetCharacter());
		}
		else {
			buffer[*length] = character->GetCharacters()[0];
			(*length)++;
			buffer[*length] = character->GetCharacters()[1];
			this->rowLength += characterMetrics->GetWidth(128);
		}
		(*length)++;
		i++;
	}
}

void Row::GetContents(Long start, Long count, Long* i, Long* j, char* contents) {
	Glyph* character;

	while (*i <= count && start < this->length) {
		character = this->GetAt(start);
		if (dynamic_cast<SingleByteCharacter*>(character)) {
			contents[*j] = character->GetCharacter();
		}
		else {
			contents[*j] = character->GetCharacters()[0];
			(*j)++;
			contents[*j] = character->GetCharacters()[1];
		}
		start++;
		(*j)++;
		(*i)++;
	}
}

Row& Row::operator=(const Row& source) {
	Composite::operator=(source);
	
	this->isSelected = source.isSelected;
	this->rowLength = source.rowLength;

	return *this;
}