//DateTimeCommand.h
#ifndef _DATATIMECOMMAND_H
#define _DATETIMECOMMAND_H

#include "UnDoReDoCommand.h"

class DateTimeCommand : public UnDoReDoCommand {

	public:
		Long startPointer;
		Long endPointer;

	public:
		DateTimeCommand(TextEditor* textEditor);
		DateTimeCommand(TextEditor* textEditor, bool isNew, bool isMacro);
		DateTimeCommand(TextEditor* textEditor, bool isNew, bool isMacro, Long row, Long column, Long afterRow, Long afterColumn, Long startPointer, Long endPointer, Long longestRow, Long hSMax);
		virtual ~DateTimeCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_DATETIMECOMMAND_H