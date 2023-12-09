//ReplaceCurrentCommand.h
#ifndef _REPLACECURRENTCOMMAND_H
#define _REPLACECURRENTCOMMAND_H

#include "UnDoReDoCommand.h"

class ReplaceCurrentCommand : public UnDoReDoCommand {

	public:
		ReplaceCurrentCommand(TextEditor* textEditor);
		ReplaceCurrentCommand(TextEditor* textEditor, bool isNew, bool isMacro, Command* pCommand);
		ReplaceCurrentCommand(TextEditor* textEditor, bool isNew, bool isMacro, Command* pCommand,
			Long row, Long column, Position startPosition, Position endPosition, Long longestRow, Long hSMax, CString findWhat, CString replaceWith, Long replaceWithLength);
		virtual ~ReplaceCurrentCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_REPLACECURRENTCOMMAND_H