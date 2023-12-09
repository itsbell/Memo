//MouseWheelCommand.h
#ifndef _MOUSEWHEELCOMMAND_H
#define _MOUSEWHEELCOMMAND_H

#include "Command.h"

class MouseWheelCommand : public Command {

	public:
		MouseWheelCommand(TextEditor* textEditor);
		virtual ~MouseWheelCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_MOUSEWHEELCOMMAND_H
