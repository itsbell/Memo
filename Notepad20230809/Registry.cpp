//Registry.cpp
#include "Registry.h"
#include "Notepad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Registry::Registry() {
	//ÄÄÇ»ÅÍ\HKEY_CURRENT_USER\Software\Memo\Memo
}

Registry::~Registry() {

}

bool Registry::GetClientPosition(RECT* rect) {
	bool ret;
	double* pData;
	UINT bytes;

	ret = AfxGetApp()->GetProfileBinary("Setting", "clientPosition", (LPBYTE*)&pData, &bytes);
	memcpy(rect, pData, bytes);
	delete[] pData;

	return ret;
}

bool Registry::GetFont(LOGFONT* lf) {
	bool ret;
	double* pData;
	UINT bytes;
	
	ret = AfxGetApp()->GetProfileBinary("Setting", "lf", (LPBYTE*)&pData, &bytes);
	memcpy(lf, pData, bytes);
	delete[] pData;

	return ret;
}

bool Registry::GetOnWordWrap() {
	bool onWordWrap;

	onWordWrap = AfxGetApp()->GetProfileInt("Setting", "onWordWrap", 0);

	return onWordWrap;
}

bool Registry::GetOnStatusBar() {
	bool onStatusBar;

	onStatusBar = AfxGetApp()->GetProfileIntA("Setting", "onStatusBar", true);

	return onStatusBar;
}

bool Registry::SetClientPosition(Notepad* notepad) {
	bool ret;
	RECT rect;

	notepad->GetWindowRect(&rect);
	ret = AfxGetApp()->WriteProfileBinary("Setting", "clientPosition", (LPBYTE)&rect, sizeof(rect));

	return ret;
}

bool Registry::SetFont(LOGFONT* lf) {
	bool ret;

	ret = AfxGetApp()->WriteProfileBinary("Setting", "lf", (LPBYTE)lf, sizeof(*lf));

	return ret;
}

bool Registry::SetOnStatusBar(bool onStatusBar) {
	bool ret;

	ret = AfxGetApp()->WriteProfileInt("Setting", "onStatusBar", onStatusBar);

	return ret;
}

bool Registry::SetOnWordWrap(bool onWordWrap) {
	bool ret;
	
	ret = AfxGetApp()->WriteProfileInt("Setting", "onWordWrap", onWordWrap);

	return ret;
}