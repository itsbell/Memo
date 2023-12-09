//CharacterMetrics.h
#ifndef _CHARACTERMETRICS_H
#define _CHARACTERMETRICS_H

#define TAB 9
#define SPACE 32
#define ASCII 128

#include <afxwin.h>

typedef signed long int Long;

class Font;
class ZoomController;
class CharacterMetrics {

	public:
		CharacterMetrics();
		CharacterMetrics(CDC* dc, Font* font, ZoomController* zoomController);
		CharacterMetrics(HDC hDC, Font* font);
		~CharacterMetrics();

		LONG GetWidth(Long index) const;
		LONG GetHeight() const;
		
	private:
		LONG widths[ASCII + 1];
		LONG height;
		
};


inline LONG CharacterMetrics::GetWidth(Long index) const {
	return this->widths[index];
}

inline LONG CharacterMetrics::GetHeight() const {
	return this->height;
}


#endif //_CHARACTERMETRICS_H
