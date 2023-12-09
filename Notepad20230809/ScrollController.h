//ScrollController.h
#ifndef _SCROLLCONTROLLER_H
#define _SCROLLCONTROLLER_H

#include "Observer.h"

class Scroll;

class ScrollController : public Observer {

	public:
		Scroll* horizontalScroll;
		Scroll* verticalScroll;
		Long below;
		Long diskFileVSMax;
		Long vSMax;

	public:
		ScrollController(TextEditor* textEditor);
		virtual ~ScrollController();

		virtual void Update(Subject* subject);
		virtual void UpdatePosition(CharacterMetrics* characterMetrics);
		virtual void UpdateMaximum();
		virtual void UpdateHSMax();
		virtual void UpdateVSMax();
		virtual void UpdateFileVSInfo(bool needUpdateVaildMin);
		virtual Long UpdateVaildMin();
		virtual void MoveDown(Long count);
		virtual void MoveUp(Long count);
		
		int LineLeft(int distance);
		int LineRight(int distance);
		int PageLeft(int page);
		int PageRight(int page);
		int HSThumbTrack(int position);

		int LineDown(int distance);
		int LineUp(int distance);
		int PageDown(int page);
		int PageUp(int page);
		int VSThumbTrack(int position);
		
		virtual Long SetVaildMin(Long vaildMin);
		virtual Long SetBelow(Long below);
		virtual Long GetDiskFileVSMax() const;
		virtual Long GetVaildMin() const;
		virtual Long GetBelow() const;

	private:
		Long vaildMin;
};

inline Long ScrollController::GetDiskFileVSMax() const {
	return this->diskFileVSMax;
}

inline Long ScrollController::GetVaildMin() const {
	return this->vaildMin;
}
inline Long ScrollController::GetBelow() const {
	return this->below;
}

#endif //_SCROLLCONTROLLER_H