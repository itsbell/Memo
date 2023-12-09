//RowBackSpaceKeyCommand.h
#ifndef _ROWBACKSPACEKEYCOMMAND_H
#define _ROWBACKSPACEKEYCOMMAND_H

#include "UnDoReDoCommand.h"

class RowBackSpaceKeyCommand : public UnDoReDoCommand {

	public:
		RowBackSpaceKeyCommand(TextEditor* textEditor);
		RowBackSpaceKeyCommand(TextEditor* textEditor, bool isNew, bool isMacro, Long row, Long column, Long longestRow, Long hSMax);
		virtual ~RowBackSpaceKeyCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_ROWBACKSPACEKEYCOMMAND_H