//Font.h
#ifndef _FONT_H
#define _FONT_H

#include <afxwin.h>

class Font {

	public:
		Font();
		Font(LOGFONT lf);
		~Font();

		LOGFONT GetLogfont() const;

	private:
		LOGFONT lf;

};

inline LOGFONT Font::GetLogfont() const {
	return this->lf;
}

#endif //_FONT_H