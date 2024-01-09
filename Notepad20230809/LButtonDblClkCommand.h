//LButtonDblClkCommand.h
#ifndef _LBUTTONDBLCLKCOMMAND_H
#define _LBUTTONDBLCLKCOMMAND_H

#include "Command.h"

class LButtonDblClkCommand : public Command {
	
	public:
		LButtonDblClkCommand(TextEditor* textEditor);
		virtual ~LButtonDblClkCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_LBUTTONDBLCLKCOMMAND_H