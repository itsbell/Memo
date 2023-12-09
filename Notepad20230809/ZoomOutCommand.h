//ZoomOutCommand.h
#ifndef _ZOOMOUTCOMMAND_H
#define _ZOOMOUTCOMMAND_H

#include "ViewCommand.h"

class ZoomOutCommand : public ViewCommand {

	public:
		ZoomOutCommand(TextEditor* textEditor);
		virtual ~ZoomOutCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_ZOOMOUTCOMMAND_H