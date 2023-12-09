//PrintTextOutVisitor.h
#ifndef _PRINTTEXTOUTVISITOR_H
#define _PRINTTEXTOUTVISITOR_H

#include "GlyphVisitor.h"
#include <afxwin.h>

class Preview;
class Paper;
class Row;
class SingleByteCharacter;
class MultiByteCharacter;
class PrintTextOutVisitor : public GlyphVisitor {

	public:
		PrintTextOutVisitor(Preview* preview, HDC hDC, LONG startX, LONG startY);
		virtual ~PrintTextOutVisitor();

		virtual void Visit(Paper* paper);
		virtual void Visit(Row* row);
		virtual void Visit(SingleByteCharacter* singleByteCharacter);
		virtual void Visit(MultiByteCharacter* multiByteCharacter);

	private:
		Preview* preview;
		HDC hDC;
		LONG x;
		LONG y;
		LONG startX;
		LONG startY;
};

#endif //_PRINTTEXTOUTVISITOR_H