//MultiByteCharacter.cpp

#include "MultiByteCharacter.h"
#include "GlyphVisitor.h"
#include "CharacterMetrics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
클래스명: MultiByteCharacter
기    능: MultiByteCharacter -▷ Character -▷ Glyph / MultiByte 문자에 대한 클래스(Concrete Class)
*/

MultiByteCharacter::MultiByteCharacter() {
	this->isSelected = false;
}

MultiByteCharacter::MultiByteCharacter(char(*character)) {
	this->character[0] = character[0];
	this->character[1] = character[1];
	this->isSelected = false;
}

MultiByteCharacter::MultiByteCharacter(const MultiByteCharacter& source) {
	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->isSelected = source.isSelected;
}

MultiByteCharacter::~MultiByteCharacter() {

}

/*
함수명칭: Clone
기    능: 현재의 객체와 똑같은 객체를 생성하다. (관련 디자인패턴: Prototype)
입    력: 없음
출    력: Glyph Link
*/
Glyph* MultiByteCharacter::Clone() {
	return new MultiByteCharacter(*this);
}

/*
함수명칭: GetContents
기    능: 해당 객체의 내용을 구하다.
입    력: 없음
출    력: string
*/
string MultiByteCharacter::GetContents() {
	string contents("");
	
	contents += this->character[0];
	contents += this->character[1];
	
	return contents;
}

/*
함수명칭: Accept
기    능: 입력되는 GlyphVisitor의 Concrete클래스에 따라 해당 클래스의 Visit함수를 호출한다.
입    력: GlyphVisitor Link
출    력: 없음
*/
void MultiByteCharacter::Accept(GlyphVisitor* visitor) {
	visitor->Visit(this);
}

Long MultiByteCharacter::GetWidth(CharacterMetrics* characterMetrics) {
	Long width;

	width = characterMetrics->GetWidth(128);

	return width;
}

MultiByteCharacter& MultiByteCharacter::operator=(const MultiByteCharacter& source) {
	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->isSelected = source.isSelected;

	return *this;
}