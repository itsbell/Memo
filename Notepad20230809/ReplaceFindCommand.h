//ReplaceFindCommand.h
#ifndef _REPLACEFINDCOMMAND_H
#define _REPLACEFINDCOMMAND_H

#include "MacroCommand.h"

class ReplaceFindCommand : public MacroCommand {

	public:
		ReplaceFindCommand(TextEditor* textEditor);
		ReplaceFindCommand(TextEditor* textEditor, bool unDoReDo);
		virtual ~ReplaceFindCommand();

		virtual Command* EmptyClone();
};

#endif //_REPLACEFINDCOMMAND_H