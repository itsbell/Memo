//Glyph.cpp
#include "Glyph.h"
#include "Position.h"
#include <afxwin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
클래스명: Glyph
기    능: Composite 디자인 패턴을 통해 만든 Paper, Row, DummyRow, MultiByteCharacter, SingleByteCharacter의 최상위 개념 /추상클래스
*/

Glyph::Glyph() {

}

Glyph::~Glyph() {

}

void Glyph::UnSelect(char* logPath) {

}

Long Glyph::Add(Glyph* glyph) {
	return -1;
}

Long Glyph::Add(Glyph* glyph, CharacterMetrics* characterMetrics) {
	return -1;
}

Long Glyph::Add(Long index, Glyph* glyph) {
	return -1;
}

Long Glyph::Add(Long index, Glyph* glyph, CharacterMetrics* characterMetrics) {
	return -1;
}

Long Glyph::Remove(Long index) {
	return -1;
}

Long Glyph::Remove(Long index, CharacterMetrics* characterMetrics) {
	return -1;
}

void Glyph::Delete(Long start, Position startPosition, Position endPosition, bool isWrapped, CharacterMetrics* characterMetrics, Long width) {

}

Glyph* Glyph::GetAt(Long index) {
	return 0;
}

Long Glyph::First() {
	return -1;
}

Long Glyph::Last() {
	return -1;
}

Long Glyph::Previous() {
	return -1;
}

Long Glyph::Next() {
	return -1;
}

Long Glyph::Move(Long index) {
	return -1;
}

Long Glyph::Move(Long destination, CharacterMetrics* characterMetrics)
{
	return -1;
}

Long Glyph::PreviousWord() {
	return -1;
}

Long Glyph::NextWord() {
	return -1;
}

Long Glyph::GetCapacity() const {
	return -1;
}

Long Glyph::GetLength() const {
	return -1;
}

Long Glyph::GetCurrent() const {
	return -1;
}

Long Glyph::GetRowCount() const {
	return -1;
}

char Glyph::GetCharacter() const {
	return 0;
}

char* Glyph::GetCharacters() const {
	return 0;
}

Long Glyph::GetWidth(CharacterMetrics* characterMetrics) {
	return -1;
}

void Glyph::Wrap(int width, CharacterMetrics* characterMetrics) {

}

void Glyph::Wrap(int width, CharacterMetrics* characterMetrics, Long* index, Glyph** glyph) {

}

void Glyph::UnWrap(CharacterMetrics* characterMetrics) {

}

void Glyph::RowWrap(Long width, CharacterMetrics* characterMetrics) {

}

void Glyph::WrapRow(Long index, Long width, CharacterMetrics* characterMetrics, bool move) {

}

void Glyph::RowUnWrap(CharacterMetrics* characterMetrics) {

}

void Glyph::UnWrapRow(Long index, CharacterMetrics* characterMetrics) {

}

Long Glyph::GetRowNumber(Long index) {
	return -1;
}

Long Glyph::GetCloseRow(Long index) {
	return -1;
}

Long Glyph::GetRowNumber(Long start, Long index) {
	return -1;
}

Long Glyph::GetColumnNumber() {
	return -1;
}

void Glyph::SelectContents() {

}

void Glyph::SelectContents(Long start, Long end) {

}

void Glyph::ConvertChar(Long* index, Long* length, Long* rowLength, char* buffer) {

}

void Glyph::ConvertChar(Long* index, Long* length, Long* rowLength, char* buffer, CharacterMetrics* characterMetrics) {

}

void Glyph::ConvertChar(char* buffer, Long* length) {

}

void Glyph::ConvertChar(char* buffer, Long* length, CharacterMetrics* characterMetrics) {

}

bool Glyph::GetIsSelected() const {
	return false;
}

Long Glyph::GetRowLength() const {
	return -1;
}

int Glyph::CompareSelectingPart(string findWhat) {
	return -1;
}

string Glyph::Copy() {
	return "";
}

void Glyph::Paste(char* text, CharacterMetrics* characterMetrics) {

}

void Glyph::FindNext(char* findWhat, bool* found, Long* row, Long* start, Long* end) {

}

void Glyph::FindNextMatchWholeWord(char* findWhat, bool* found, Long* row, Long* start, Long* end) {

}

void Glyph::FindNextMatchCase(char* findWhat, bool* found, Long* row, Long* start, Long* end) {

}

void Glyph::FindNextMatchWholeWordAndCase(char* findWhat, bool* found, Long* row, Long* start, Long* end) {

}

void Glyph::FindPrevious(char* findWhat, bool* found, Long* row, Long* start, Long* end) {

}

void Glyph::FindPreviousMatchCase(char* findWhat, bool* found, Long* row, Long* start, Long* end) {

}

void Glyph::FindPreviousMatchWholeWord(char* findWhat, bool* found, Long* row, Long* start, Long* end) {

}

void Glyph::FindPreviousMatchWholeWordAndCase(char* findWhat, bool* found, Long* row, Long* start, Long* end) {

}

void Glyph::GetContents(Long start, Long count, Long* i, Long* j, char* contents){

}