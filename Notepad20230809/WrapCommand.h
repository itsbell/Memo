//WrapCommand.h
#ifndef _WRAPCOMMAND_H
#define _WRAPCOMMAND_H

#include "ViewCommand.h"

class TextEditor;

class WrapCommand : public ViewCommand {

	public:
		WrapCommand(TextEditor* textEditor);
		virtual ~WrapCommand();

		virtual void Execute();
		virtual void Unexecute();

};

#endif //_WRAPCOMMAND_H