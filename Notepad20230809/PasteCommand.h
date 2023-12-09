//PasteCommand.h
#ifndef _PASTECOMMAND_H
#define _PASTECOMMAND_H

#include "UnDoReDoCommand.h"

class PasteCommand : public UnDoReDoCommand {
	
	public:
		Long startPointer;
		Long endPointer;

	public:
		PasteCommand(TextEditor* textEditor);
		PasteCommand(TextEditor* textEditor, bool isNew, bool isMacro);
		PasteCommand(TextEditor* textEditor, bool isNew, bool isMacro, Long row, Long column, Long afterRow, Long afterColumn, Long startPointer, Long endPointer, Long longestRow, Long hSMax);
		virtual ~PasteCommand();

		virtual void Execute();
		virtual void Unexecute();
};


#endif //_PASTECOMMAND_H