//OpenCommand.h

#ifndef _OPENCOMMAND_H
#define _OPENCOMMAND_H
#include "FileCommand.h"

class TextEditor;

class OpenCommand : public FileCommand {

	public:
		OpenCommand(TextEditor* textEditor);
		virtual ~OpenCommand();

		virtual void Execute();
		virtual void Unexecute();

};

#endif //_OPENCOMMAND_H