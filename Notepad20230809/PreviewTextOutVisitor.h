//PreviewTextOutVisitor.h
#ifndef _PREVIEWTEXTOUTVISITOR_H
#define _PREVIEWTEXTOUTVISITOR_H

#include <afxwin.h>
#include "GlyphVisitor.h"

class Preview;
class PreviewTextOutVisitor : public GlyphVisitor {

	public:
		PreviewTextOutVisitor(Preview* preview, CDC* dc, LONG startX, LONG startY);
		virtual ~PreviewTextOutVisitor();
		
		virtual void Visit(Paper* paper);
		virtual void Visit(Row* row);
		virtual void Visit(SingleByteCharacter* singleByteCharacter);
		virtual void Visit(MultiByteCharacter* multiByteCharacter);

	private:
		Preview* preview;
		CDC* dc;
		LONG x;
		LONG y;
		LONG startX;
		LONG startY;
};

#endif //_PREVIEWTEXTOUTVISITOR_H