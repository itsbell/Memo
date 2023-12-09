//Observer.h
#ifndef _OBSERVER_H
#define _OBSERVER_H

typedef signed long int Long;

class Subject;
class TextEditor;
class CharacterMetrics;
class Observer {

	public:
		Observer();
		virtual ~Observer() = 0;

		virtual void Update(Subject* subject) = 0;
		virtual void TakeIn(int minimuum);
		virtual void TakeOut();
		virtual void Save();
		virtual void SaveRowLength();
		virtual void SaveRowLength(bool isWrapped);
		virtual void UpdateMaximum();
		virtual void UpdateHSMax();
		virtual void UpdateVSMax();
		virtual void UpdateFileVSInfo(bool needUpdateVaildMin);
		virtual Long UpdateVaildMin();
		virtual void UpdatePosition(CharacterMetrics* characterMetrics);
		virtual void MoveUp(Long count);
		virtual void MoveDown(Long count);
		virtual void ScrollUp(Long position, Long page, Long vaildMin, Long vaildMax);
		virtual void ScrollUp(bool isWrapped, Long position, Long page, Long vaildMin, Long vaildMax);
		virtual void ScrollDown(Long position, Long page, Long vaildMin, Long vaildMax);
		virtual void ScrollDown(bool isWrapped, Long position, Long page, Long vaildMin, Long vaildMax);

		virtual Long SetVaildMin(Long vaildMin);
		virtual Long SetBelow(Long below);


	protected:
		TextEditor* textEditor;

};

#endif //_OBSERVER_H
