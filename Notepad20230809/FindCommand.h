//FindCommand.h
#ifndef _FINDCOMMAND_H
#define _FINDCOMMAND_H

#include "EditCommand.h"

class FindCommand : public EditCommand {

	public:
		FindCommand(TextEditor* textEditor);
		virtual ~FindCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_FINDCOMMAND_H