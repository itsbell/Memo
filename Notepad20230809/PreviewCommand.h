//PreviewCommand.h
#ifndef _PREVIEWCOMMAND_H
#define _PREVIEWCOMMAND_H

#include "FileCommand.h"

class PreviewCommand : public FileCommand {

	public:
		PreviewCommand(TextEditor* textEditor);
		virtual ~PreviewCommand();

		virtual void Execute();
		virtual void Unexecute();
};

#endif //_PREVIEWCOMMAND_H