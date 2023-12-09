//HScrollActionFactory.h
#ifndef _HSCROLLACTIONFACTORY_H
#define _HSCROLLACTIONFACTORY_H

#include <afxwin.h>

class TextEditor;
class HScrollAction;

class HScrollActionFactory {

	public:
		HScrollActionFactory(TextEditor* textEditor);
		~HScrollActionFactory();

		HScrollAction* Create(UINT nSBCode);

	private:
		TextEditor* textEditor;
};

#endif //_HSCROLLACTIONFACTORY_H