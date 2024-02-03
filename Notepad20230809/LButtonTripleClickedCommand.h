//LButtonTripleClickedCommand.h
#ifndef _LBUTTONTRIPLECLICKEDCOMMAND_H
#define _LBUTTONTRIPLECLICKEDCOMMAND_H

#include "Command.h"

class LButtonTripleClickedCommand : public Command {

	public:
		LButtonTripleClickedCommand(TextEditor* textEditor);
		virtual ~LButtonTripleClickedCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_LBUTTONTRIPLECLICKEDCOMMAND_H