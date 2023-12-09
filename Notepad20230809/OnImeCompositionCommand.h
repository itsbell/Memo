//OnImeCompositionCommand.h
#ifndef _ONIMECOMPOSITIONCOMMAND_H
#define _ONIMECOMPOSITIONCOMMAND_H

#include "UnDoReDoCommand.h"

class OnImeCompositionCommand : public UnDoReDoCommand {
	
	public:
		OnImeCompositionCommand(TextEditor* textEditor);
		OnImeCompositionCommand(TextEditor* textEditor, bool isNew, bool macro);
		virtual ~OnImeCompositionCommand();

		virtual void Execute();
		virtual void Unexecute();

};

#endif //_ONIMECOMPOSITIONCOMMAND_H