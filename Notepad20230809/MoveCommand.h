//MoveCommand.h
#ifndef _MOVECOMMAND_H
#define _MOVECOMMAND_H

#include "EditCommand.h"

class MoveCommand : public EditCommand {

	public:
		MoveCommand(TextEditor* textEditor);
		virtual ~MoveCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_MOVECOMMAND_H