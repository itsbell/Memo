//Document.h
#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#include <string>
using namespace std;
#include "Position.h"

typedef signed long int Long;

class CharacterMetrics;
class Glyph;
class FileMaker;
class Document {

	public:
		Glyph* paper;
		Position startPosition;
		Position endPosition;
		bool isSelecting;
		
	public:
		Document();
		~Document();

		void AddDeletedContents(Position start, Position end, Long* startPointer, Long* endPointer, FileMaker* fm);
		string Copy(Long row);
		void Copy(Long row, FILE* file, char* contents, Long* length);
		void CountRows(Long* line, Long count, Long rectRight, CharacterMetrics* characterMetrics, Long* total, FileMaker* fm);
		void CountRows(bool reverse, Long* line, Long count, Long rectRight, CharacterMetrics* characterMetrics, Long* total, FileMaker* fm);
		void CountRows(Long start, Long number, Long below, Long rectRight, CharacterMetrics* characterMetrics, Long* row, Long* count, FileMaker* fm);
		bool Compare(char* findWhat, Position start, Position end, bool matchCase, FileMaker* fm);
		void Correct(char* buffer, Long start, Long end, CharacterMetrics* characterMetrics, char* contents, char* replaceWith, Long* byte, Long* rowLength, Long* index);
		void Delete(bool isWrapped, Long width, CharacterMetrics* characterMetrics, FileMaker* fm);
		void FindNext(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm);
		void FindNextMatchWholeWord(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm);
		void FindNextMatchCase(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm);
		void FindNextMatchWholeWordAndCase(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm);
		void FindPrevious(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm);
		void FindPreviousMatchCase(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm);
		void FindPreviousMatchWholeWord(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm);
		void FindPreviousMatchWholeWordAndCase(char* findWhat, Long startRow, Long startColumn, bool* found, Long* row, Long* start, Long* end, FileMaker* fm);
		void FindNext(char* buffer, Long index, char* findWhat, bool* found, Long* start, Long* end);
		void FindNextMatchWholeWord(char* buffer, Long index, char* findWhat, bool* found, Long* start, Long* end);
		void FindNextMatchCase(char* buffer, Long index, char* findWhat, bool* found, Long* start, Long* end);
		void FindNextMatchWholeWordAndCase(char* buffer, Long index, char* findWhat, bool* found, Long* start, Long* end);
		Long GetIndex(char* text, Long column);
		Long GetRowNumber();
		Long GetRowNumber(bool rowOrDummy, Long row);
		Position GetPosition();
		void Load(string pathName, CharacterMetrics* characterMetrics);
		void LoadToMemory(Long start, Long end, CharacterMetrics* characterMetrics, FileMaker* fm);
		void LoadToMemory(Long start, Long end, CharacterMetrics* characterMetrics, Long width, FileMaker* fm);
		void LoadToMemory(bool appendFromFront, Long start, Long end, CharacterMetrics* characterMetrics, FileMaker* fm);
		void LoadToMemory(bool appendFromFront, Long start, Long end, CharacterMetrics* characterMetrics, Long width, FileMaker* fm);
		void Move(Long row, Long column);
		void Move(Long row, Long column, char* logPath);
		void OnlySelect(Position previous, Position current);
		void PasteFromFile(string fileName, Long start, Long end, CharacterMetrics* characterMetrics, bool isWrapped, Long width);
		void Select(Position previous, Position current);
		Long SetEnd(Long end);
		Long SetLength(Long length);
		void SetSelectingInfo(Position previous, Position current);
		void SetSelectingInfo(bool isSelecting, Position previous, Position current);
		Long SetStart(Long start);
		void SelectAll();
		void SelectLoadDatas(Long start, Long end);
		void UnSelect();
		void UnWrap(CharacterMetrics* characterMetrics, FileMaker* fm);
		void UpdateSelectingInfo(Position previous, Position current);
		void Wrap(Long width, CharacterMetrics* characterMetrics, FileMaker* fm);

		Long GetLength() const;
		Long GetStart() const;
		Long GetEnd() const;

	private:
		Long length;
		Long start;
		Long end;
};

inline Long Document::GetLength() const {
	return this->length;
}

inline Long Document::GetStart() const {
	return this->start;
}

inline Long Document::GetEnd() const {
	return this->end;
}

#endif //_DOCUMENT_H