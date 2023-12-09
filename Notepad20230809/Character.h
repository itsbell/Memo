//Character.h
#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "Glyph.h"

class Character : public Glyph {
	
	public:
		Character();
		virtual ~Character() = 0;

		virtual void Select();
		virtual void UnSelect();
		virtual bool GetIsSelected() const;

	protected:
		bool isSelected;
};

inline bool Character::GetIsSelected() const {
	return this->isSelected;
}

#endif //_CHARACTER_H
