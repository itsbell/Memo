//LButtonDownCommand.h
#ifndef _LBUTTONDOWNCOMMAND_H
#define _LBUTTONDOWNCOMMAND_H

#include "Command.h"

class LButtonDownCommand : public Command {
	
	public:
		LButtonDownCommand(TextEditor* textEditor);
		virtual ~LButtonDownCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_LBUTTONDOWNCOMMAND_H
