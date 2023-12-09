//ReplaceCommand.h
#ifndef _REPLACECOMMAND_H
#define _REPLACECOMMAND_H

#include "EditCommand.h"

class ReplaceCommand : public EditCommand {

	public:
		ReplaceCommand(TextEditor* textEditor);
		virtual ~ReplaceCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_REPLACECOMMAND_H