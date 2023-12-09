//Command.h
#ifndef _COMMAND_H
#define _COMMAND_H

class TextEditor;
class Command {
	
	public:
		Command();
		virtual ~Command() = 0;

		virtual void Execute() = 0;
		virtual void Unexecute() = 0;

	protected:
		TextEditor* textEditor;
};

#endif //_COMMAND_H