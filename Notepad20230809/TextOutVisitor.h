//TextOutVisitor.h
#ifndef _TEXTOUTVISITOR_H
#define _TEXTOUTVISITOR_H

#define TAB 9

#include <afxwin.h>
#include "GlyphVisitor.h"

typedef signed long int Long;

class TextEditor;
class Paper;
class Row;
class SingleByteCharacter;
class MultiByteCharacter;
class ScrollController;
class TextOutVisitor : public GlyphVisitor {

	public:
		TextOutVisitor(TextEditor* textEditor, CDC* dc, int y, Long start, Long end);
		virtual ~TextOutVisitor();

		virtual void Visit(Paper* paper);
		virtual void Visit(Row* row);
		virtual void Visit(SingleByteCharacter* singleByteCharacter);
		virtual void Visit(MultiByteCharacter* multiByteCharacter);

	private:
		CDC* dc;
		int x;
		int y;
		Long start;
		Long end;
		int hSPosition;
		int hSPage;
		int characterHeight;
};

#endif //_TEXTOUTVISITOR_H