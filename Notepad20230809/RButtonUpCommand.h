//RButtonUpCommand.h
#ifndef _RBUTTONUPCOMMAND_H
#define _RBUTTONUPCOMMAND_H

#include "Command.h"

class RButtonUpCommand : public Command {

	public:
		RButtonUpCommand(TextEditor* textEditor);
		virtual ~RButtonUpCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_RBUTTONUPCOMMAND_H
