//UTF16BEConverter.cpp
#include "UTF16BEConverter.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UTF16BEConverter::UTF16BEConverter(Document* document){
	this->document = document;
	this->data = 0;
	this->fileLength = 0;
}

UTF16BEConverter::UTF16BEConverter(Document* document, TCHAR* data, Long fileLength) {
	this->document = document;
	this->data = data;
	this->fileLength = fileLength;
}

UTF16BEConverter::~UTF16BEConverter() {

}

void UTF16BEConverter::EncodeToWideChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, Long* wLength)
{
	WCHAR ch;
	WCHAR tmpBuf[MAX];

	*wLength = MultiByteToWideChar(CP_ACP, 0, (char*)buffer, byte, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, (char*)buffer, byte, pBuffer, *wLength);
	memset(tmpBuf, 0, MAX);
	memmove(tmpBuf, pBuffer, (*wLength) * 2);
	int nPos = 0;
	while (nPos <= *wLength) {
		ch = pBuffer[nPos];
		if (ch == 0x00) {
			memcpy(pBuffer, tmpBuf, (*wLength) * 2);
			break;
		}
		tmpBuf[nPos++] = MAKEWORD(HIBYTE(ch), LOBYTE(ch));
	}
}

char* UTF16BEConverter::Decode() {
	int nPos = 0;
	int wLength;
	int mLength;
	WCHAR ch;
	WCHAR* pBuffer = new WCHAR[fileLength + 2];
	WCHAR* tmpBuf = new WCHAR[fileLength + 2];
	
	wcscpy_s(pBuffer, fileLength + 2, (WCHAR*)data + 1);
	memset(tmpBuf, 0, fileLength + 2);
	memmove(tmpBuf, pBuffer, fileLength);
	while (nPos <= fileLength) {
		ch = pBuffer[nPos];
		if (ch == 0x00) {
			memcpy(pBuffer, tmpBuf, fileLength);
			break;
		}
		tmpBuf[nPos++] = MAKEWORD(HIBYTE(ch), LOBYTE(ch));
	}
	wLength = nPos;
	
	mLength = WideCharToMultiByte(CP_ACP, 0, pBuffer, wLength, NULL, 0, NULL, NULL);
	char* buffer = new char[mLength + 1];
	memset(buffer, 0x00, sizeof(char) * (mLength + 1));
	mLength = WideCharToMultiByte(CP_ACP, 0, pBuffer, wLength, buffer, mLength, NULL, NULL);
	buffer[mLength] = 0;

	if (tmpBuf != 0) {
		delete[] tmpBuf;
	}
	if (pBuffer != 0) {
		delete[] pBuffer;
	}
	
	return buffer;
}