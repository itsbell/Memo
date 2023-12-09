//UTF16LEConverter.cpp
#include "UTF16LEConverter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UTF16LEConverter::UTF16LEConverter(Document* document){
	this->document = document;
	this->data = 0;
	this->fileLength = 0;
}

UTF16LEConverter::UTF16LEConverter(Document* document, TCHAR* data, Long fileLength) {
	this->document = document;
	this->data = data;
	this->fileLength = fileLength;
}

UTF16LEConverter::~UTF16LEConverter() {

}

void UTF16LEConverter::EncodeToWideChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, Long* wLength) 
{
	*wLength = MultiByteToWideChar(CP_ACP, 0, (char*)buffer, byte, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, (char*)buffer, byte, pBuffer, *wLength);
}

char* UTF16LEConverter::Decode() {
	int mLength;
	int wLength;
	WCHAR* pBuffer = new WCHAR[fileLength + 2];

	wcscpy_s(pBuffer, fileLength + 2, (WCHAR*)data + 1);
	wLength = wcslen(pBuffer);

	mLength = WideCharToMultiByte(CP_ACP, 0, pBuffer, wLength, NULL, 0, NULL, NULL);
	char* buffer = new char[mLength + 1];
	memset(buffer, 0x00, sizeof(char) * (mLength + 1));
	mLength = WideCharToMultiByte(CP_ACP, 0, pBuffer, wLength, buffer, mLength, NULL, NULL);
	buffer[mLength] = '\0';

	if (pBuffer != 0) {
		delete[] pBuffer;
	}

	return buffer;
}