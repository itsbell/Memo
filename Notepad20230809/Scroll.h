//Scroll.h
#ifndef _SCROLL_H
#define _SCROLL_H

#include <afxwin.h>

class TextEditor;

class Scroll {

	public: 
		Scroll();
		Scroll(int minimum, int maximum, UINT page, int position);
		virtual ~Scroll() = 0;

		virtual int SetPosition(int position);
		virtual int SetMaximum(int maximum);

		virtual int GetMinimum() const;
		virtual int GetMaximum() const;
		virtual UINT GetPage() const;
		virtual int GetPosition() const;
		

	protected:
		int minimum;
		int maximum;
		UINT page;
		int position;
};

inline int Scroll::GetMinimum() const {
	return this->minimum;
}

inline int Scroll::GetMaximum() const {
	return this->maximum;
}

inline UINT Scroll::GetPage() const {
	return this->page;
}

inline int Scroll::GetPosition() const {
	return this->position;
}

#endif //_SCROLL_H