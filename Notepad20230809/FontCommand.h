//FontCommand.h
#ifndef _FONTCOMMAND_H
#define _FONTCOMMAND_H

#include "EditCommand.h"

class TextEditor;

class FontCommand : public EditCommand {

	public: 
		FontCommand(TextEditor* textEditor);
		virtual ~FontCommand();

		virtual void Execute();
		virtual void Unexecute();

};

#endif //_FONTCOMMAND_H