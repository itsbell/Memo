//SaveCommand.h
#ifndef _SAVECOMMAND_H
#define _SAVECOMMAND_H
#include "FileCommand.h"

class TextEditor;

class SaveCommand : public FileCommand {

	public: 
		SaveCommand(TextEditor* textEditor);
		virtual ~SaveCommand();

		virtual void Execute();
		virtual void Unexecute();

};

#endif //_SAVECOMMAND_H