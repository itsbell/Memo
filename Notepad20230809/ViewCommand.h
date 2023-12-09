//ViewCommand.h
#ifndef _VIEWCOMMAND_H
#define _VIEWCOMMAND_H

#include "Command.h"

class ViewCommand : public Command {

	public:
		ViewCommand();
		virtual ~ViewCommand() = 0;
};

#endif //_VIEWCOMMAND_H