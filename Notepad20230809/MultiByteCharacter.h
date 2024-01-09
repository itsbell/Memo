//MultiByteCharacter.h
#ifndef _MULTIBYTECHARACTER_H
#define _MULTIBYTECHARACTER_H

#include "Character.h"
#include <string>
using namespace std;

class GlyphVisitor;
class MultiByteCharacter : public Character {

	public:
		MultiByteCharacter();
		MultiByteCharacter(char(*character));
		MultiByteCharacter(const MultiByteCharacter& source);
		virtual ~MultiByteCharacter();
		
		virtual Glyph* Clone();
		virtual int Compare(char character);
		virtual int Compare(char* character);
		virtual string GetContents();
		virtual void Accept(GlyphVisitor* visitor);
		virtual Long GetWidth(CharacterMetrics* characterMetrics);

		MultiByteCharacter& operator =(const MultiByteCharacter& source);
		
		char* GetCharacters() const;

	private:
		char character[2];
};

inline char* MultiByteCharacter::GetCharacters() const {
	return const_cast<char*>(this->character);
}

#endif //_MULTIBYTECHARACTER_H
