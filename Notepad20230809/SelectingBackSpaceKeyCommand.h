//SelectingBackSpaceKeyCommand.h
#ifndef _SELECTINGBACKSPACEKEYCOMMAND_H
#define _SELECTINGBACKSPACEKEYCOMMAND_H

#include "Command.h"

class SelectingBackSpaceKeyCommand : public Command {

	public:
		SelectingBackSpaceKeyCommand(TextEditor* textEditor);
		virtual ~SelectingBackSpaceKeyCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_SELECTINGBACKSPACEKEYCOMMAND_H