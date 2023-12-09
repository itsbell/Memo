//RowDeleteKeyCommand.h
#ifndef _ROWDELETEKEYCOMMAND_H
#define _ROWDELETEKEYCOMMAND_H

#include "UnDoReDoCommand.h"

class RowDeleteKeyCommand : public UnDoReDoCommand {

	public:
		RowDeleteKeyCommand(TextEditor* textEditor);
		RowDeleteKeyCommand(TextEditor* textEditor, Long row, Long column, Long longestRow, Long hSMax);
		virtual ~RowDeleteKeyCommand();
		
		virtual void Execute();
		virtual void Unexecute();
};

#endif _ROWDELETEKEYCOMMAND_H