//OnCharSelectingCommand.h
#ifndef _ONCHARSELECTINGCOMMAND_H
#define _ONCHARSELECTINGCOMMAND_H

#include "MacroCommand.h"


class OnCharSelectingCommand : public MacroCommand {

	public:
		OnCharSelectingCommand(TextEditor* textEditor);
		OnCharSelectingCommand(TextEditor* textEditor, bool unDoReDo);
		virtual ~OnCharSelectingCommand();

		virtual Command* EmptyClone();
};

#endif //_ONCHARSELECTINGCOMMAND_H