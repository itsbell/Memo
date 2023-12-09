//InfoCommand.h
#ifndef _INFOCOMMAND_H
#define _INFOCOMMAND_H

#include "HelpCommand.h"

class InfoCommand : public HelpCommand {

	public:
		InfoCommand(TextEditor* textEditor);
		virtual ~InfoCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_INFOCOMMAND_H