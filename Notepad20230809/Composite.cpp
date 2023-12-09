//Composite.cpp
#include "Composite.h"
#include "Paper.h"
#include "Row.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Ŭ������: Composite
��    ��: Composite -�� Glyph / Composite ������ �������� ������� �߻�Ŭ����
*/

/*
�Լ���Ī: Composite(������)
��    ��: Composite�� ����� �޴� Concrete Class���� ������
��    ��: �Ҵ緮
��    ��: ����
*/
Composite::Composite(Long capacity)
	:glyphs(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->current = 0;
}

/*
�Լ���Ī: Composite(���������)
��    ��: Composite�� ����� �޴� Concrete Class���� ���������
��    ��: source
��    ��: ����
*/
Composite::Composite(const Composite& source)
	: glyphs(source.glyphs) {
	Glyph* glyph;
	Long i = 0;

	//1. source�� ��뷮 ��ŭ �ݺ��ϴ�.
	while (i < source.length) {
		
		//1.1. ������ �ν��Ͻ��� �迭�� i��° ���� ���� �����ϴ�. (Prototype ������ ����)
		glyph = const_cast<Composite&>(source).glyphs[i]->Clone();

		//1.2. ���� ������ ���� this �迭�� i��° ������ ��ġ��.
		this->glyphs.Modify(i, glyph);

		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;
}

/*
�Լ���Ī: ~Composite(�Ҹ���)
��    ��: Composite�� ����� �޴� Concrete Class���� �Ҹ���
��    ��: ����
��    ��: ����
*/
Composite::~Composite() {
	Long i = 0;

	while (i < this->length) {
		if (this->glyphs[i] != 0) {
			delete this->glyphs[i];
		}
		i++;
	}
}

/*
�Լ���Ī: Add
��    ��: Array<Glyph*> glyphs�� front�� glyph�� �߰��ϴ�.
��    ��: Glyph Link
��    ��: ��ġ
*/
Long Composite::Add(Glyph* glyph) {
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

	return index;
}

/*
�Լ���Ī: Add
��    ��: Array<Glyph*> glyphs�� front�� index��°�� glyph�� �߰��ϴ�.
��    ��: Glyph Link
��    ��: ��ġ
*/
Long Composite::Add(Long index, Glyph* glyph) {

	index = this->glyphs.Insert(index, glyph);

	this->capacity++;
	this->length++;
	this->current = index + 1;

	return index;
}

/*
�Լ���Ī: Remove
��    ��: Array<Glyph*> glyphs�� front�� index��° ���� �����.
��    ��: ��ġ
��    ��: ��ġ
*/
Long Composite::Remove(Long index) {

	this->current = index;

	if (dynamic_cast<Paper*>(this) && this->current < 1) {
		this->current = 1;	
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

/*
�Լ���Ī: Move
��    ��: ���� ��ġ�� �̵��ϴ�.
��    ��: ��ġ
��    ��: ��ġ
*/
Long Composite::Move(Long index) {

	this->current = index;

	return this->current;
}

/*
�Լ���Ī: GetAt
��    ��: Array<Glyph*> glyphs�� front���� �Էµ� index��°�� ���� ���ϴ�.
��    ��: index
��    ��: Glyph Link
*/
Glyph* Composite::GetAt(Long index) {
	return this->glyphs.GetAt(index);
}

/*
�Լ���Ī: operator = 
��    ��: ġȯ�����ڷ� Composite�� ���� ConcreteŬ������ ġȯ�Ͽ� �����ϴ�.
��    ��: source
��    ��: ����
*/
Composite& Composite::operator=(const Composite& source) {
	Glyph* glyph;
	Long i = 0;

	//1. glyphs �迭�� source�� glyphs �迭�� ġȯ�ϴ�.
	this->glyphs = source.glyphs;
	
	//2. source�� glyphs �迭 ��뷮 ��ŭ �ݺ��ϴ�.
	while (i < source.length) {

		//2.1. source�� glyphs �迭�� i��° ���� ���� ���� �ϴ�. (���� ���������� prototype)
		glyph = const_cast<Composite&>(source).glyphs[i]->Clone();

		//2.2. source�� glyphs �迭�� i��° ���� ��ġ��.
		this->glyphs.Modify(i, glyph);

		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;

	return *this;
}
