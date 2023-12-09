//SelectAllCommand.h
#ifndef _SELECTALLCOMMAND_H
#define _SELECTALLCOMMAND_H

#include "EditCommand.h"

class SelectAllCommand : public EditCommand {
	
	public:
		SelectAllCommand(TextEditor* textEditor);
		virtual ~SelectAllCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_SELECTALLCOMMAND_H