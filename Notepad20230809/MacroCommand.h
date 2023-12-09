//MacroCommand.h
#ifndef _MACROCOMMAND_H
#define _MACROCOMMAND_H

#include "Command.h"
#include "Array.h"

typedef signed long int Long;

class MacroCommand : public Command {

	public:
		bool unDoReDo;

	public:
		MacroCommand(Long capacity = 256);
		virtual ~MacroCommand();
		
		virtual Long Add(Command* command);
		virtual Long Remove(Long index);
		virtual void Execute();
		virtual void Unexecute();

		virtual Command* EmptyClone() = 0;
		
	private:
		Array<Command*> commands;
		Long capacity;
		Long length;
		
};

#endif //_MACROCOMMAND_H