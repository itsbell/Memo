//UnDoCommand.h
#ifndef _UNDOCOMMAND_H
#define _UNDOCOMMAND_H

#include "Command.h"

class UnDoCommand : public Command {

	public:
		UnDoCommand(TextEditor* textEditor);
		virtual ~UnDoCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_UNDOCOMMAND_H