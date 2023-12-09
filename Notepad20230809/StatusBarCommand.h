//StatusBarCommand.h
#ifndef _STATUSBARCOMMAND_H
#define _STATUSBARCOMMAND_H

#include "ViewCommand.h"

class StatusBarCommand : public ViewCommand {

	public:
		StatusBarCommand(TextEditor* textEditor);
		virtual ~StatusBarCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_STATUSBARCOMMAND_H