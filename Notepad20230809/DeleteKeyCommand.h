//DeleteKeyCommand.h
#ifndef _DELETEKEYCOMMAND_H
#define _DELETEKEYCOMMAND_H

#include "Command.h"

class DeleteKeyCommand : public Command {

	public:
		DeleteKeyCommand(TextEditor* textEditor);
		virtual ~DeleteKeyCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_DELETEKEYCOMMAND_H