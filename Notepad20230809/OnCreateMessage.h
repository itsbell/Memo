//OnCreateMessage.h
#ifndef _ONCREATEMESSAGE_H
#define _ONCREATEMESSAGE_H
#include "Command.h"
class TextEditor;

class OnCreateMessage : public Command{

	public:
		OnCreateMessage(TextEditor* textEditor);
		virtual ~OnCreateMessage();

		virtual void Execute();
		virtual void Unexecute();

};

#endif //_ONCREATEMESSAGE_H