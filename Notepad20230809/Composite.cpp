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
클래스명: Composite
기    능: Composite -▷ Glyph / Composite 디자인 패턴으로 만들어진 추상클래스
*/

/*
함수명칭: Composite(생성자)
기    능: Composite의 상속을 받는 Concrete Class들의 생성자
입    력: 할당량
출    력: 없음
*/
Composite::Composite(Long capacity)
	:glyphs(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->current = 0;
}

/*
함수명칭: Composite(복사생성자)
기    능: Composite의 상속을 받는 Concrete Class들의 복사생성자
입    력: source
출    력: 없음
*/
Composite::Composite(const Composite& source)
	: glyphs(source.glyphs) {
	Glyph* glyph;
	Long i = 0;

	//1. source의 사용량 만큼 반복하다.
	while (i < source.length) {
		
		//1.1. 복사할 인스턴스의 배열의 i번째 값을 복사 생성하다. (Prototype 디자인 패턴)
		glyph = const_cast<Composite&>(source).glyphs[i]->Clone();

		//1.2. 복사 생성한 값을 this 배열의 i번째 값으로 고치다.
		this->glyphs.Modify(i, glyph);

		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;
}

/*
함수명칭: ~Composite(소멸자)
기    능: Composite의 상속을 받는 Concrete Class들의 소멸자
입    력: 없음
출    력: 없음
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
함수명칭: Add
기    능: Array<Glyph*> glyphs의 front에 glyph를 추가하다.
입    력: Glyph Link
출    력: 위치
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
함수명칭: Add
기    능: Array<Glyph*> glyphs의 front에 index번째에 glyph를 추가하다.
입    력: Glyph Link
출    력: 위치
*/
Long Composite::Add(Long index, Glyph* glyph) {

	index = this->glyphs.Insert(index, glyph);

	this->capacity++;
	this->length++;
	this->current = index + 1;

	return index;
}

/*
함수명칭: Remove
기    능: Array<Glyph*> glyphs의 front에 index번째 값을 지우다.
입    력: 위치
출    력: 위치
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
함수명칭: Move
기    능: 현재 위치를 이동하다.
입    력: 위치
출    력: 위치
*/
Long Composite::Move(Long index) {

	this->current = index;

	return this->current;
}

/*
함수명칭: GetAt
기    능: Array<Glyph*> glyphs의 front에서 입력된 index번째의 값을 구하다.
입    력: index
출    력: Glyph Link
*/
Glyph* Composite::GetAt(Long index) {
	return this->glyphs.GetAt(index);
}

/*
함수명칭: operator = 
기    능: 치환연산자로 Composite의 하위 Concrete클래스를 치환하여 대입하다.
입    력: source
출    력: 없음
*/
Composite& Composite::operator=(const Composite& source) {
	Glyph* glyph;
	Long i = 0;

	//1. glyphs 배열을 source의 glyphs 배열로 치환하다.
	this->glyphs = source.glyphs;
	
	//2. source의 glyphs 배열 사용량 만큼 반복하다.
	while (i < source.length) {

		//2.1. source의 glyphs 배열의 i번째 값을 복사 생성 하다. (관련 디자인패턴 prototype)
		glyph = const_cast<Composite&>(source).glyphs[i]->Clone();

		//2.2. source의 glyphs 배열의 i번째 값을 고치다.
		this->glyphs.Modify(i, glyph);

		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;

	return *this;
}
