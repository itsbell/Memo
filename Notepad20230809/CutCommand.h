//CutCommand.h
#ifndef _CUTCOMMAND_H
#define _CUTCOMMAND_H

#include "MacroCommand.h"

class TextEditor;

class CutCommand : public MacroCommand {

	public:
		CutCommand(TextEditor* textEditor);
		CutCommand(TextEditor* textEditor, bool unDoReDo);
		virtual ~CutCommand();

		virtual Command* EmptyClone();
};

#endif //_CUTCOMMAND_H