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
Ŭ������: SingleByteCharacter
��    ��: SingleByteCharacter -�� Character -�� Glyph / SingleByte ���ڿ� ���� Ŭ����(Concrete Class)
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
�Լ���Ī: Clone
��    ��: ������ ��ü�� �Ȱ��� ��ü�� �����ϴ�. (���� ����������: Prototype)
��    ��: ����
��    ��: Glyph Link
*/
Glyph* SingleByteCharacter::Clone() {
	return new SingleByteCharacter(*this);
}

/*
�Լ���Ī: GetContents
��    ��: �ش� ��ü�� ������ ���ϴ�.
��    ��: ����
��    ��: string
*/
string SingleByteCharacter::GetContents() {
	string contents("");

	contents += this->GetCharacter();

	return contents;
}

/*
�Լ���Ī: Accept
��    ��: �ԷµǴ� GlyphVisitor�� ConcreteŬ������ ���� �ش� Ŭ������ Visit�Լ��� ȣ���Ѵ�.
��    ��: GlyphVisitor Link
��    ��: ����
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