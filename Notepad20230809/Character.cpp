//Character.cpp
#include "Character.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
클래스명: Character
기    능: Glyph로 부터 상속 받는 Character(문자) 추상클래스
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