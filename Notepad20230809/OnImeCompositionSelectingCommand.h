//OnImeCompositionSelectingCommand.h
#ifndef _ONIMECOMPOSITIONSELECTINGCOMMAND_H
#define _ONIMECOMPOSITIONSELECTINGCOMMAND_H

#include "MacroCommand.h"

class OnImeCompositionSelectingCommand : public MacroCommand {
	
	public:
		OnImeCompositionSelectingCommand(TextEditor* textEditor);
		OnImeCompositionSelectingCommand(TextEditor* textEditor, bool unDoReDo);
		virtual ~OnImeCompositionSelectingCommand();

		virtual Command* EmptyClone();
};

#endif //_ONIMECOMPOSITIONSELECTINGCOMMAND_H