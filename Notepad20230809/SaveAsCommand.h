//SaveAsCommand.h
#ifndef _SAVEASCOMMAND_H
#define _SAVEASCOMMAND_H
#include "FileCommand.h"

class TextEditor;

class SaveAsCommand : public FileCommand {

	public:	
		SaveAsCommand(TextEditor* textEditor);
		virtual ~SaveAsCommand();

		virtual void Execute();
		virtual void Unexecute();

};

#endif //_SAVEASCOMMAND_H