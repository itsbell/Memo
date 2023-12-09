//SelectingDateTimeCommand.h
#ifndef _SELECTINGDATETIMECOMMAND_H
#define _SELECTINGDATETIMECOMMAND_H

#include "MacroCommand.h"

class SelectingDateTimeCommand : public MacroCommand {

	public:
		SelectingDateTimeCommand(TextEditor* textEditor);
		SelectingDateTimeCommand(TextEditor* textEditor, bool unDoReDo);
		virtual ~SelectingDateTimeCommand();

		virtual Command* EmptyClone();
};

#endif //_SELECTINGDATETIMECOMMAND_H