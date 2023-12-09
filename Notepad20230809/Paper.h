//Paper.h
#ifndef _PAPER_H
#define _PAPER_H

#include "Composite.h"
#include <string>
using namespace std;

class Position;
class GlyphVisitor;
class Paper : public Composite {

	public:
		Paper(Long capacity = 256);
		Paper(const Paper& source);
		virtual ~Paper();

		virtual Long Add(Glyph* glyph);
		virtual Long Add(Long index, Glyph* glyph);
		virtual Long Remove(Long index);
		virtual void Delete(Long start, Position startPosition, Position endPosition, bool isWrapped, CharacterMetrics* characterMetrics, Long width);

		virtual Glyph* Clone();
		virtual string GetContents();
		virtual void ConvertChar(Long* index, Long* length, Long* rowLength, char* buffer);
		virtual void ConvertChar(Long* index, Long* length, Long* rowLength, char* buffer, CharacterMetrics* characterMetrics);
		virtual void Accept(GlyphVisitor* visitor);
	
		virtual void Wrap(int width, CharacterMetrics* characterMetrics);
		virtual void UnWrap(CharacterMetrics* characterMetrics);
		virtual void RowWrap(Long width, CharacterMetrics* characterMetrics);
		virtual void WrapRow(Long index, Long width, CharacterMetrics* characterMetrics, bool move);
		virtual void RowUnWrap(CharacterMetrics* characterMetrics);
		virtual void UnWrapRow(Long index, CharacterMetrics* characterMetrics);
		virtual Long GetRowNumber(Long index);
		virtual Long GetCloseRow(Long index);
		virtual Long GetRowNumber(Long start, Long index);
		virtual Long GetColumnNumber();

		virtual Long First();
		virtual Long Last();
		virtual Long Previous();
		virtual Long Next();

		virtual void Select();
		virtual void UnSelect();
		virtual int CompareSelectingPart(string findWhat);

		virtual string Copy();
		virtual void Paste(char* text, CharacterMetrics* characterMetrics);
		
		virtual void FindNext(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindNextMatchWholeWord(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindNextMatchCase(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindNextMatchWholeWordAndCase(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindPrevious(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindPreviousMatchCase(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindPreviousMatchWholeWord(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindPreviousMatchWholeWordAndCase(char* findWhat, bool* found, Long* row, Long* start, Long* end);

		Paper& operator=(const Paper& source);

		virtual Long GetRowCount() const;

	private:
		Long rowCount;
};

inline Long Paper::GetRowCount() const{
	return this->rowCount;
}

#endif //_PAPER_H