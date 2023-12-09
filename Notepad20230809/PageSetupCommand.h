//PageSetupCommand.h
#ifndef _PAGESETUPCOMMAND_H
#define _PAGESETUPCOMMAND_H

#include "FileCommand.h"

class PageSetupCommand : public FileCommand {

	public:
		PageSetupCommand(TextEditor* textEditor);
		virtual ~PageSetupCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_PAGESETUPCOMMAND_H