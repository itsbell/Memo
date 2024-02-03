//Row.h
#ifndef _ROW_H
#define _ROW_H

#define ASCII 128

#include "Composite.h"
#include <string>
using namespace std;

class GlyphVisitor;
class CharacterMetrics;

class Row : public Composite {

	public:
		Long rowLength;

	public:
		Row(Long capacity = 256);
		Row(const Row& source);
		virtual ~Row();
		
		virtual Long Add(Glyph* glyph, CharacterMetrics* characterMetrics);
		virtual Long Add(Long index, Glyph* glyph, CharacterMetrics* characterMetrics);
		virtual Long Remove(Long index, CharacterMetrics* characterMetrics);
		virtual Glyph* Clone();
		virtual string GetContents();
		virtual string GetContents(Long startColumn, Long endColumn);
		virtual void GetContents(Long start, Long count, Long* i, Long* j, char* contents);
		virtual void Accept(GlyphVisitor* visitor);

		virtual Long First();
		virtual Long Last();
		virtual Long Previous();
		virtual Long Next();
		virtual Long Move(Long destination, CharacterMetrics* characterMetrics);
		virtual Long PreviousWord();
		virtual Long NextWord();
		
		virtual string Copy();
		virtual Long GetWidth(CharacterMetrics* characterMetrics);
		virtual void Wrap(int width, CharacterMetrics* characterMetrics, Long* index, Glyph** glyph);
		virtual void Select();
		virtual void SelectContents();
		virtual void SelectContents(Long start, Long end);
		virtual void UnSelect();
		virtual void ConvertChar(char* buffer, Long* length);
		virtual void ConvertChar(char* buffer, Long* length, CharacterMetrics* characterMetrics);

		Row& operator=(const Row& source);

		virtual bool GetIsSelected() const;
		virtual Long GetRowLength() const;

	private:
		bool isSelected;
};

inline bool Row::GetIsSelected() const {
	return this->isSelected;
}

inline Long Row::GetRowLength() const {
	return this->rowLength;
}

#endif //_ROW_H
