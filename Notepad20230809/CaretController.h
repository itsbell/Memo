//CaretController.h
#ifndef _CARETCONTROLLER_H
#define _CARETCONTROLLER_H

#include "Observer.h"

class TextEditor;
class Subject;

class CaretController : public Observer{

	public: 
		CaretController(TextEditor* textEditor);
		virtual ~CaretController();

		virtual void Update(Subject* subject);

};

#endif //_CARETCONTROLLER_H