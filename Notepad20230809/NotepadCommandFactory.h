//NotepadCommandFactory.h
#ifndef _NOTEPADCOMMANDFACTORY_H
#define _NOTEPADCOMMANDFACTORY_H

#include "Notepad.h"

class Command;
class NotepadCommandFactory {
	
	public:
		NotepadCommandFactory(Notepad* notepad);
		~NotepadCommandFactory();

		Command* Create(UINT nID);

	private:
		Notepad* notepad;
};

#endif //_NOTEPADCOMMANDFACTORY_H