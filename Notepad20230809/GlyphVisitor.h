//GlyphVisitor.h

#ifndef _GLYPHVISITOR_H
#define _GLYPHVISITOR_H

class TextEditor;
class Paper;
class Row;
class SingleByteCharacter;
class MultiByteCharacter;

class GlyphVisitor {

	public:
		GlyphVisitor();
		virtual ~GlyphVisitor() = 0;

		virtual void Visit(Paper* paper) = 0;
		virtual void Visit(Row* row) = 0;
		virtual void Visit(SingleByteCharacter* singleByteCharacter) = 0;
		virtual void Visit(MultiByteCharacter* multiByteCharacter) = 0;

	protected:
		TextEditor* textEditor;
};

#endif //_GLYPHVISITOR_H