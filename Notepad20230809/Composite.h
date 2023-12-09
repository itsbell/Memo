//Composite.h
#ifndef _COMPOSITE_H
#define _COMPOSITE_H

#include "Glyph.h"
#include "Array.h"

typedef signed long int Long;

class Composite : public Glyph {

	public:
		Composite(Long capacity = 256);
		Composite(const Composite& source);
		virtual ~Composite() = 0;

		virtual Long Add(Glyph* glyph);
		virtual Long Add(Long index, Glyph* glyph);
		virtual Long Move(Long index);
		virtual Long Remove(Long index);
		virtual Glyph* GetAt(Long index);

		Composite& operator = (const Composite& source);

		virtual Long GetCapacity() const;
		virtual Long GetLength() const;
		virtual Long GetCurrent() const;

	protected:
		Array<Glyph*> glyphs;
		Long capacity;
		Long length;
		Long current;
};


inline Long Composite::GetCapacity() const {
	return this->capacity;
}

inline Long Composite::GetLength() const {
	return this->length;
}

inline Long Composite::GetCurrent() const {
	return this->current;
}

#endif //_COMPOSITE_H