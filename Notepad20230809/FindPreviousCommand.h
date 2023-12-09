//FindPreviousCommand.h
#ifndef _FINDPREVIOUSCOMMAND_H
#define _FINDPREVIOUSCOMMAND_H

#include "Command.h"

class FindPreviousCommand : public Command {

	public:
		FindPreviousCommand(TextEditor* textEditor);
		virtual ~FindPreviousCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_FINDPREVIOUSCOMMAND_H