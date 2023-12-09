//ShowHelpCommand.h
#ifndef _SHOWHELPCOMMAND_H
#define _SHOWHELPCOMMAND_H

#include "HelpCommand.h"

class ShowHelpCommand :public HelpCommand {
	
	public:
		ShowHelpCommand(TextEditor* textEditor);
		virtual ~ShowHelpCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_SHOWHELPCOMMAND_H