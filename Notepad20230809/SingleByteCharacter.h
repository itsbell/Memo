//SingleByteCharacter.h
#ifndef _SINGLEBYTECHARACTER_H
#define _SINGLEBYTECHARACTER_H

#include "Character.h"
#include <string>
using namespace std;

class GlyphVisitor;
class SingleByteCharacter : public Character {

	public:
		SingleByteCharacter();
		SingleByteCharacter(char character);
		SingleByteCharacter(const SingleByteCharacter& source);
		virtual ~SingleByteCharacter();
		
		virtual Glyph* Clone();
		virtual int Compare(char character);
		virtual int Compare(char* character);
		virtual string GetContents();
		virtual void Accept(GlyphVisitor* visitor);
		virtual Long GetWidth(CharacterMetrics* characterMetrics);

		SingleByteCharacter& operator=(const SingleByteCharacter& source);
		
		char GetCharacter() const;

	private:
		char character;

};

inline char SingleByteCharacter::GetCharacter() const {
	return this->character;
}

#endif //_SINGLEBYTECHARACTER_H
