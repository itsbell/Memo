//CharacterDeleteKeyCommand.h
#ifndef _CHARACTERDELETEKEYCOMMAND_H
#define _CHARACTERDELETEKEYCOMMAND_H

#include "UnDoReDoCommand.h"

class CharacterDeleteKeyCommand : public UnDoReDoCommand {
	
	public:
		CharacterDeleteKeyCommand(TextEditor* textEditor);
		CharacterDeleteKeyCommand(TextEditor* textEditor, Long row, Long column, char* character, Long longestRow, Long hSMax);
		virtual ~CharacterDeleteKeyCommand();

		virtual void Execute();
		virtual void Unexecute();
};


#endif //_CHARACTERDELETEKEYCOMMAND_H