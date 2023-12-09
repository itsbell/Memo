//OnCharEnterSelectingCommand.h
#ifndef _ONCHARENTERSELECTINGCOMMAND_H
#define _ONCHARENTERSELECTINGCOMMAND_H

#include "MacroCommand.h"

class OnCharEnterSelectingCommand : public MacroCommand {

	public:
		OnCharEnterSelectingCommand(TextEditor* textEditor);
		OnCharEnterSelectingCommand(TextEditor* textEditor, bool empty);
		virtual ~OnCharEnterSelectingCommand();

		virtual Command* EmptyClone();
};

#endif //ONCHARENTERSELECTINGCOMMAND_H