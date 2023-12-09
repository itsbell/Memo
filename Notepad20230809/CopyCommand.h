//CopyCommand.h
#ifndef _COPYCOMMAND_H
#define _COPYCOMMAND_H

#include "UnDoReDoCommand.h"

class CopyCommand : public UnDoReDoCommand {

	public:
		CopyCommand(TextEditor* textEditor);
		CopyCommand(TextEditor* textEditor, bool isNew, bool isMacro);
		virtual ~CopyCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_COPYCOMMAND_H