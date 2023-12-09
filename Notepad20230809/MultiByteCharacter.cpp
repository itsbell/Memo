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
Ŭ������: MultiByteCharacter
��    ��: MultiByteCharacter -�� Character -�� Glyph / MultiByte ���ڿ� ���� Ŭ����(Concrete Class)
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
�Լ���Ī: Clone
��    ��: ������ ��ü�� �Ȱ��� ��ü�� �����ϴ�. (���� ����������: Prototype)
��    ��: ����
��    ��: Glyph Link
*/
Glyph* MultiByteCharacter::Clone() {
	return new MultiByteCharacter(*this);
}

/*
�Լ���Ī: GetContents
��    ��: �ش� ��ü�� ������ ���ϴ�.
��    ��: ����
��    ��: string
*/
string MultiByteCharacter::GetContents() {
	string contents("");
	
	contents += this->character[0];
	contents += this->character[1];
	
	return contents;
}

/*
�Լ���Ī: Accept
��    ��: �ԷµǴ� GlyphVisitor�� ConcreteŬ������ ���� �ش� Ŭ������ Visit�Լ��� ȣ���Ѵ�.
��    ��: GlyphVisitor Link
��    ��: ����
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