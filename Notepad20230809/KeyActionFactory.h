//KeyActionFactory.h
#ifndef _KEYACTIONFACTORY_H
#define _KEYACTIONFACTORY_H

#include <afxwin.h>

class KeyAction;
class TextEditor;

class KeyActionFactory {

	public:
		TextEditor* textEditor;

	public:
		KeyActionFactory();
		KeyActionFactory(TextEditor* textEditor);
		~KeyActionFactory();

		KeyAction* Create(UINT nChar);

};

#endif //_KEYACTIONFACTORY_H