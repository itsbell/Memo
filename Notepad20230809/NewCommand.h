//NewCommand.h
#ifndef _NEWCOMMAND_H
#define _NEWCOMMAND_H

#include "FileCommand.h"

class NewCommand : public FileCommand {

	public:
		NewCommand(TextEditor* textEditor);
		virtual ~NewCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_NEWCOMMAND_H