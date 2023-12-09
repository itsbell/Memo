//LButtonUpCommand.h
#ifndef _LBUTTONUPCOMMAND_H
#define _LBUTTONUPCOMMAND_H

#include "Command.h"

class LButtonUpCommand : public Command {

	public:
		LButtonUpCommand(TextEditor* textEdior);
		virtual ~LButtonUpCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_LBUTTONUPCOMMAND_H