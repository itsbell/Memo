//HelpCommand.h
#ifndef _HELPCOMMAND_H
#define _HELPCOMMAND_H

#include "Command.h"

class HelpCommand : public Command {
	
	public:
		HelpCommand();
		virtual ~HelpCommand() = 0;

};

#endif //_HELPCOMMAND_H