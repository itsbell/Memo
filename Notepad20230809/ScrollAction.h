//ScrollAction.h
#ifndef _SCROLLACTION_H
#define _SCROLLACTION_H

#include <afxwin.h>

class TextEditor;

class ScrollAction {

	public:
		ScrollAction();
		virtual ~ScrollAction() = 0;

	protected:
		TextEditor* textEditor;
};

#endif _SCROLLACTION_H