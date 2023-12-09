//EditCommand.h
#ifndef _EDITCOMMAND_H
#define _EDITCOMMAND_H

#include "Command.h"

class EditCommand : public Command {

	public:
		EditCommand();
		virtual ~EditCommand() = 0;
};

#endif //_EDITCOMMAND_H