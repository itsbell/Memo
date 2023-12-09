//OnCharCommand.h
#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "UnDoReDoCommand.h"

class OnCharCommand : public UnDoReDoCommand {
	
	public:
		OnCharCommand(TextEditor* textEditor);
		OnCharCommand(TextEditor* textEditor, bool isNew, bool macro);
		OnCharCommand(TextEditor* textEditor, bool macro, char *character, Long row, Long column);
		virtual ~OnCharCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_ONCHARCOMMAND_H