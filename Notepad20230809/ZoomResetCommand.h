//ZoomResetCommand.h
#ifndef _ZOOMRESETCOMMAND_H
#define _ZOOMRESETCOMMAND_H

#include "ViewCommand.h"

class ZoomResetCommand : public ViewCommand {

	public:
		ZoomResetCommand(TextEditor* textEditor);
		virtual ~ZoomResetCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_ZOOMRESETCOMMAND_H