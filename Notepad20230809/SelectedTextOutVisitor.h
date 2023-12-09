//SelectedTextOutVisitor.h
#ifndef _SELECTEDTEXTOUTVISITOR_H
#define _SELECTEDTEXTOUTVISITOR_H

#include <afxwin.h>
#include "GlyphVisitor.h"

typedef signed long int Long;

class SelectedTextOutVisitor : public GlyphVisitor {

	public:
		SelectedTextOutVisitor(TextEditor* textEditor, CDC* dc, int y, Long start, Long end);
		virtual ~SelectedTextOutVisitor();
		
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

#endif //_SELECTEDTEXTOUTVISITOR_H