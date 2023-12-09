//OnImeCharCommand.h
#ifndef _ONIMECHARCOMMAND_H
#define _ONIMECHARCOMMAND_H

#include "UnDoReDoCommand.h"

class OnImeCharCommand : public UnDoReDoCommand {
	
	public:
		OnImeCharCommand(TextEditor* textEditor);
		OnImeCharCommand(TextEditor* textEditor, char* character, Long row, Long column);
		virtual ~OnImeCharCommand();

		virtual void Execute();
		virtual void Unexecute();

};

#endif //_ONIMECHARCOMMAND_H