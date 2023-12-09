//BackSpaceKeyCommand.h
#ifndef _BACKSPACEKEYCOMMAND_H
#define _BACKSPACEKEYCOMMAND_H

#include "Command.h"

class BackSpaceKeyCommand : public Command {
	
	public:
		BackSpaceKeyCommand(TextEditor* textEditor);
		virtual ~BackSpaceKeyCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_BACKSPACEKEYCOMMAND_H