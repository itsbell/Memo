//Glyph.h
#ifndef _GLYPH_H
#define _GLYPH_H

#include <string>
using namespace std;

typedef signed long int Long;

class Position;
class TextEditor;
class CharacterMetrics;
class GlyphVisitor;

class Glyph {

	public:
		Glyph();
		virtual ~Glyph() = 0;

		virtual Glyph* Clone() = 0;
		virtual string GetContents() = 0;
		virtual void GetContents(Long start, Long count, Long* i, Long* j, char* contents);
		virtual void Accept(GlyphVisitor* visitor) = 0;
		virtual void Select() = 0;
		virtual void UnSelect() = 0;
		virtual void UnSelect(char* logPath);

		virtual Long Add(Glyph* glyph);
		virtual Long Add(Glyph* glyph, CharacterMetrics* characterMetrics);
		virtual Long Add(Long index, Glyph* glyph);
		virtual Long Add(Long index, Glyph* glyph, CharacterMetrics* characterMetrics);

		virtual Long Remove(Long index);
		virtual Long Remove(Long index, CharacterMetrics* characterMetrics);
		virtual void Delete(Long start, Position startPosition, Position endPosition, bool isWrapped, CharacterMetrics* characterMetrics, Long width);

		virtual Glyph* GetAt(Long index);
		
		virtual Long First();
		virtual Long Last();
		virtual Long Previous();
		virtual Long Next();
		virtual Long Move(Long index);
		virtual Long Move(Long destination, CharacterMetrics* characterMetrics);
		virtual Long PreviousWord();
		virtual Long NextWord();

		virtual void Wrap(int width, CharacterMetrics* characterMetrics);
		virtual void Wrap(int width, CharacterMetrics* characterMetrics, Long* index, Glyph** glyph);
		virtual void UnWrap(CharacterMetrics* characterMetrics);
		virtual void RowWrap(Long width, CharacterMetrics* characterMetrics);
		virtual void WrapRow(Long index, Long width, CharacterMetrics* characterMetrics, bool move);
		virtual void RowUnWrap(CharacterMetrics* characterMetrics);
		virtual void UnWrapRow(Long index, CharacterMetrics* characterMetrics);
		virtual Long GetRowNumber(Long index);
		virtual Long GetCloseRow(Long index);
		virtual Long GetRowNumber(Long start, Long index);
		virtual Long GetColumnNumber();
		virtual void SelectContents();
		virtual void SelectContents(Long start, Long end);

		virtual void ConvertChar(Long* index, Long* length, Long* rowLength, char* buffer);
		virtual void ConvertChar(Long* index, Long* length, Long* rowLength, char* buffer, CharacterMetrics* characterMetrics);
		virtual void ConvertChar(char* buffer, Long* length);
		virtual void ConvertChar(char* buffer, Long* length, CharacterMetrics* characterMetrics);

		virtual int CompareSelectingPart(string findWhat);

		virtual void FindNext(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindNextMatchWholeWord(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindNextMatchCase(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindNextMatchWholeWordAndCase(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindPrevious(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindPreviousMatchCase(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindPreviousMatchWholeWord(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		virtual void FindPreviousMatchWholeWordAndCase(char* findWhat, bool* found, Long* row, Long* start, Long* end);
		
		virtual Long GetWidth(CharacterMetrics *characterMetrics);
		
		virtual string Copy();
		virtual void Paste(char* text, CharacterMetrics* characterMetrics);

		virtual Long GetCapacity() const;
		virtual Long GetLength() const;
		virtual Long GetCurrent() const;
		virtual Long GetRowCount() const;
		virtual bool GetIsSelected() const;
		virtual Long GetRowLength() const;
		virtual char GetCharacter() const;
		virtual char* GetCharacters() const;

};

#endif //_GLYPH_H