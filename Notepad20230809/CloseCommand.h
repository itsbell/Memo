//CloseCommand.h
#ifndef _CLOSECOMMAND_H
#define _CLOSECOMMAND_H

#include "FileCommand.h"

class CloseCommand : public FileCommand {

	public:
		CloseCommand(TextEditor* textEditor);
		virtual ~CloseCommand();
		virtual void Execute();
		virtual void Unexecute();
};

#endif //_CLOSECOMMAND_H