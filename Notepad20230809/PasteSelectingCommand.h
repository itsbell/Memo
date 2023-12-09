//PasteSelectingCommand.h
#ifndef _PASTESELECTINGCOMMAND_H
#define _PASTESELECTINGCOMMAND_H

#include "MacroCommand.h"

class PasteSelectingCommand : public MacroCommand {

	public:
		PasteSelectingCommand(TextEditor* textEditor);
		PasteSelectingCommand(TextEditor* textEditor, bool unDoReDo);
		virtual ~PasteSelectingCommand();

		virtual Command* EmptyClone();
};

#endif //_PASTESELECTINGCOMMAND_H