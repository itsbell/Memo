//FileCommand.h
#ifndef _FILECOMMAND_H
#define _FILECOMMAND_H

#include "Command.h"

class FileCommand : public Command {

	public:
		FileCommand();
		virtual ~FileCommand() = 0;
};

#endif //_FILECOMMAND_H