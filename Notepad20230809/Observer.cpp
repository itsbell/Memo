//Observer.cpp
#include "Observer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Ŭ������: Observer
��    ��: Observer ������ ������ �ֻ��� Ŭ����
*/


Observer::Observer() {

}

Observer::~Observer() {

}

void Observer::TakeIn(int minimuum)
{
}

void Observer::TakeOut()
{
}


void Observer::Save() {

}

void Observer::SaveRowLength() {

}

void Observer::SaveRowLength(bool isWrapped) {

}

void Observer::UpdateMaximum() {

}

void Observer::UpdateHSMax() {

}

void Observer::UpdateVSMax() {

}

void Observer::UpdateFileVSInfo(bool needUpdateVaildMin) {
	
}

Long Observer::UpdateVaildMin() {
	return -1;
}

void Observer::UpdatePosition(CharacterMetrics* characterMetrics) {

}

void Observer::MoveUp(Long count) {

}

void Observer::MoveDown(Long count) {

}

void Observer::ScrollUp(Long position, Long page, Long vaildMin, Long vaildMax) {

}

void Observer::ScrollUp(bool isWrapped, Long position, Long page, Long vaildMin, Long vaildMax) {

}

void Observer::ScrollDown(Long position, Long page, Long vaildMin, Long vaildMax) {

}

void Observer::ScrollDown(bool isWrapped, Long position, Long page, Long vaildMin, Long vaildMax) {

}

Long Observer::SetVaildMin(Long vaildMin) {
	return -1;
}

Long Observer::SetBelow(Long below) {
	return -1;
}