//Registry.h
#ifndef _REGISTRY_H
#define _REGISTRY_H

#include <afxwin.h>
class Notepad;

class Registry {

	public:
		Registry();
		~Registry();

		bool GetFont(LOGFONT* lf);
		bool GetOnWordWrap();
		bool GetOnStatusBar();
		bool GetClientPosition(RECT* rect);
		bool SetFont(LOGFONT* lf);
		bool SetOnWordWrap(bool onWordWrap);
		bool SetOnStatusBar(bool onStatusBar);
		bool SetClientPosition(Notepad* notepad);

};

#endif //_REGISTRY_H