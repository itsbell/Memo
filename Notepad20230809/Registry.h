//Registry.h
#ifndef _REGISTRY_H
#define _REGISTRY_H

#include <afxwin.h>

class Registry {

	public:
		Registry();
		~Registry();

		bool GetFont(LOGFONT* lf);
		bool GetIsWrapped();
		bool SetFont(LOGFONT* lf);
		bool SetIsWrapped(bool isWrapped);

};

#endif //_REGISTRY_H