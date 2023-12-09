//VScrollActionFactory.h
#ifndef _VSCROLLACTIONFACTORY_H
#define _VSCROLLACTIONFACTORY_H

#include <afxwin.h>

class TextEditor;
class VScrollAction;

class VScrollActionFactory {

	public:
		VScrollActionFactory(TextEditor* textEditor);
		~VScrollActionFactory();

		VScrollAction* Create(UINT nSBCode);

	private:
		TextEditor* textEditor;
};

#endif //_VSCROLLACTIONFACTORY_H