//GlyphFactory.h
#ifndef _GLYPHFACTORY_H
#define _GLYPHFACTORY_H

#define ASCII 128

class Glyph;
class GlyphFactory {

	public:
		GlyphFactory();
		~GlyphFactory();

		Glyph* Create(char(*contents));
};

#endif //_GLYPHFACTORY_H