//OnCharEnterCommand.h
#ifndef _ONCHARENTERCOMMAND_H
#define _ONCHARENTERCOMMAND_H

#include "UnDoReDoCommand.h"

class OnCharEnterCommand : public UnDoReDoCommand {

	public:
		OnCharEnterCommand(TextEditor* textEditor);
		OnCharEnterCommand(TextEditor* textEditor, bool macro);
		OnCharEnterCommand(TextEditor* textEditor, bool macro, Long row, Long column);
		virtual ~OnCharEnterCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_ONCHARENTERCOMMAND_H