//SingleByteCharacter.cpp
#include "SingleByteCharacter.h"
#include "GlyphVisitor.h"
#include "CharacterMetrics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
클래스명: SingleByteCharacter
기    능: SingleByteCharacter -▷ Character -▷ Glyph / SingleByte 문자에 대한 클래스(Concrete Class)
*/

SingleByteCharacter::SingleByteCharacter() {
	this->isSelected = false;
}

SingleByteCharacter::SingleByteCharacter(char character) {
	this->character = character;
	this->isSelected = false;
}

SingleByteCharacter::SingleByteCharacter(const SingleByteCharacter& source) {
	this->character = source.character;
	this->isSelected = source.isSelected;
}

SingleByteCharacter::~SingleByteCharacter() {

}

/*
함수명칭: Clone
기    능: 현재의 객체와 똑같은 객체를 생성하다. (관련 디자인패턴: Prototype)
입    력: 없음
출    력: Glyph Link
*/
Glyph* SingleByteCharacter::Clone() {
	return new SingleByteCharacter(*this);
}

/*
함수명칭: GetContents
기    능: 해당 객체의 내용을 구하다.
입    력: 없음
출    력: string
*/
string SingleByteCharacter::GetContents() {
	string contents("");

	contents += this->GetCharacter();

	return contents;
}

/*
함수명칭: Accept
기    능: 입력되는 GlyphVisitor의 Concrete클래스에 따라 해당 클래스의 Visit함수를 호출한다.
입    력: GlyphVisitor Link
출    력: 없음
*/
void SingleByteCharacter::Accept(GlyphVisitor* visitor) {
	visitor->Visit(this);
}

Long SingleByteCharacter::GetWidth(CharacterMetrics* characterMetrics) {
	Long width;
	
	width = characterMetrics->GetWidth(this->character);

	return width;
}

SingleByteCharacter& SingleByteCharacter::operator=(const SingleByteCharacter& source) {
	
	this->character = source.character;
	this->isSelected = source.isSelected;

	return *this;
}