//MemoryController.h
#ifndef _MEMORYCONTROLLER_H
#define _MEMORYCONTROLLER_H

#include "Observer.h"

typedef signed long int Long;

class TextEditor;
class Subject;
class MemoryController : public Observer {

	public:
		MemoryController(TextEditor* textEditor);
		virtual ~MemoryController();

		virtual void Update(Subject* subject);
		virtual void Save();
		virtual void SaveRowLength();
		virtual void SaveRowLength(bool isWrapped);
		virtual void TakeIn(int minimum);
		virtual void TakeOut();
		virtual void MoveUp(Long rowNumber);
		virtual void MoveDown(Long rowNumber);
		virtual void ScrollUp(Long position, Long page, Long vaildMin, Long vaildMax);
		virtual void ScrollUp(bool isWrapped, Long position, Long page, Long vaildMin, Long vaildMax);
		virtual void ScrollDown(Long position, Long page, Long vaildMin, Long vaildMax);
		virtual void ScrollDown(bool isWrapped, Long position, Long page, Long vaildMin, Long vaildMax);
};

#endif //_MEMORYCONTROLLER_H