//CharacterBackSpaceKeyCommand.h
#ifndef _CHARACTERBACKSPACEKEYCOMMAND_H
#define _CHARACTERBACKSPACEKEYCOMMAND_H

#include "UnDoReDoCommand.h"

class CharacterBackSpaceKeyCommand : public UnDoReDoCommand {

	public:
		CharacterBackSpaceKeyCommand(TextEditor* textEditor);
		CharacterBackSpaceKeyCommand(TextEditor* textEditor, bool isNew, bool isMacro, Long row, Long column, char* character, Long longestRow, Long hSMax);
		virtual ~CharacterBackSpaceKeyCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_CHARACTERBACKSPACEKEYCOMMAND_H