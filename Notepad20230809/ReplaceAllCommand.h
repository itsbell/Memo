//ReplaceAllCommand.h
#ifndef _REPLACEALLCOMMAND_H
#define _REPLACEALLCOMMAND_H

#include "UnDoReDoCommand.h"
#include "ReplaceInfoStack.h"

class ReplaceAllCommand : public UnDoReDoCommand {

	public:
		ReplaceAllCommand(TextEditor* textEditor);
		ReplaceAllCommand(const ReplaceAllCommand& source);
		virtual ~ReplaceAllCommand();

		virtual void Execute();
		virtual void Unexecute();

	private:
		ReplaceInfoStack* unDoStack;
		ReplaceInfoStack* reDoStack;
		CString findWhat;
		CString replaceWith;
};

#endif //_REPLACEALLCOMMAND_H