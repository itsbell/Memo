//FindNextCommand.h
#ifndef _FINDNEXTCOMMAND_H
#define _FINDNEXTCOMMAND_H

#include "UnDoReDoCommand.h"

class FindNextCommand : public UnDoReDoCommand {

	public:
		FindNextCommand(TextEditor* textEditor);
		FindNextCommand(TextEditor* textEditor, bool isNew, bool isMacro, Command* pCommand);
		virtual ~FindNextCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_FINDNEXTCOMMAND_H