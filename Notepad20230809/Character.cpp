//Character.cpp
#include "Character.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Ŭ������: Character
��    ��: Glyph�� ���� ��� �޴� Character(����) �߻�Ŭ����
*/

Character::Character() {

}

Character::~Character() {

}

void Character::Select() {
	this->isSelected = true;
}

void Character::UnSelect() {
	this->isSelected = false;
}