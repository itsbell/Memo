//DeleteCommand.h
#ifndef _DELETECOMMAND_H
#define _DELETECOMMAND_H

#include "UnDoReDoCommand.h"
#include "Position.h"

class DeleteCommand : public UnDoReDoCommand {

	public:
		Long startPointer;
		Long endPointer;

	public:
		DeleteCommand(TextEditor* textEditor);
		DeleteCommand(TextEditor* textEditor, bool macro);
		DeleteCommand(TextEditor* textEditor, bool isNew, bool isMacro, Position startPosition, Position endPosition, Long startPointer, Long endPointer, Long longestRow, Long hSMax);
		virtual ~DeleteCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_DELETECOMMAND_H