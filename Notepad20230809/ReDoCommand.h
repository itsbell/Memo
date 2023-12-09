//ReDoCommand.h
#ifndef _REDOCOMMAND_H
#define _REDOCOMMAND_H

#include "Command.h"

class ReDoCommand : public Command {

	public:
		ReDoCommand(TextEditor* textEditor);
		virtual ~ReDoCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_REDOCOMMAND_H