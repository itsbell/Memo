//ZoomInCommand.h
#ifndef _ZOOMINCOMMAND_H
#define _ZOOMINCOMMAND_H

#include "ViewCommand.h"

class ZoomInCommand : public ViewCommand {

	public:
		ZoomInCommand(TextEditor* textEditor);
		virtual ~ZoomInCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_ZOOMINCOMMAND_H