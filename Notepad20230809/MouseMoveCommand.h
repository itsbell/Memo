//MouseMoveCommand.h
#ifndef _MOUSEMOVECOMMAND_H
#define _MOUSEMOVECOMMAND_H

#include "Command.h"

class MouseMoveCommand : public Command {

	public:
		MouseMoveCommand(TextEditor* textEditor);
		virtual ~MouseMoveCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_MOUSEMOVECOMMAND_H
