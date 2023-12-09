//ExitCommand.h
#ifndef _EXITCOMMAND_H
#define _EXITCOMMAND_H

#include "FileCommand.h"

class ExitCommand : public FileCommand {
	
	public:
		ExitCommand(TextEditor* textEditor);
		virtual ~ExitCommand();
		virtual void Execute();
		virtual void Unexecute();
};

#endif //_EXITCOMMAND_H