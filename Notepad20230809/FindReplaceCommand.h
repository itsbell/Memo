//FindReplaceCommand.h
#ifndef _FINDREPLACECOMMAND_H
#define _FINDREPLACECOMMAND_H

#include "Command.h"

class FindReplaceCommand : public Command {

	public:
		FindReplaceCommand(TextEditor* textEditor);
		virtual  ~FindReplaceCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_FINDREPLACECOMMAND_H
