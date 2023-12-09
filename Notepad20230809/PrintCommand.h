//PrintCommand.h
#ifndef _PRINTCOMMAND_H
#define _PRINTCOMMAND_H

#include "FileCommand.h"

class PrintCommand : public FileCommand {

	public:
		PrintCommand(TextEditor* textEditor);
		virtual ~PrintCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_PRINTCOMMAND_H