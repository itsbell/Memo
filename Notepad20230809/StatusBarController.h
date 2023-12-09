//StatusBarController.h
#ifndef _STATUSBARCONTROLLER_H
#define _STATUSBARCONTROLLER_H

#include "Observer.h"

class TextEditor;
class Subject;
class StatusBarController : public Observer {

	public:
		StatusBarController(TextEditor* textEditor);
		virtual ~StatusBarController();

		virtual void Update(Subject* subject);
};

#endif //_STATUSBARCONTROLLER_H