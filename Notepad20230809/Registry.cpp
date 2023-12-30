//Registry.cpp
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Registry::Registry() {

}

Registry::~Registry() {

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

bool Registry::GetIsWrapped() {
	bool isWrapped;

	isWrapped = AfxGetApp()->GetProfileInt("Setting", "isWrapped", 0);

	return isWrapped;
}

bool Registry::SetFont(LOGFONT* lf) {
	bool ret;

	ret = AfxGetApp()->WriteProfileBinary("Setting", "lf", (LPBYTE)lf, sizeof(*lf));

	return ret;
}

bool Registry::SetIsWrapped(bool isWrapped) {
	bool ret;
	
	ret = AfxGetApp()->WriteProfileInt("Setting", "isWrapped", isWrapped);	

	return ret;
}