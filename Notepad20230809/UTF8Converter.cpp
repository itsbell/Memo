//UTF8Converter.cpp
#include "UTF8Converter.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UTF8Converter::UTF8Converter(Document* document) {
	this->document = document;
	this->data = 0;
	this->fileLength = 0;
}

UTF8Converter::UTF8Converter(Document* document, TCHAR* data, Long fileLength) {
	this->document = document;
	this->data = data;
	this->fileLength = fileLength;
}

UTF8Converter::~UTF8Converter() {

}

void UTF8Converter::EncodeToMultiByteChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, char* data, Long* mLength){
	int wLength = MultiByteToWideChar(CP_ACP, 0, (char*)buffer, byte, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, (char*)buffer, byte, pBuffer, wLength);
	*mLength = WideCharToMultiByte(CP_UTF8, 0, pBuffer, wLength, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, pBuffer, wLength, data, *mLength, NULL, NULL);
}

char* UTF8Converter::Decode() {
	int wLength;
	int mLength;
	WCHAR* pBuffer = new WCHAR[fileLength + 2];
	
	wLength = MultiByteToWideChar(CP_UTF8, 0, (char*)data, fileLength, NULL, NULL);
	memset(pBuffer, 0, sizeof(WCHAR) * (wLength + 1));
	MultiByteToWideChar(CP_UTF8, 0, (char*)data, fileLength, pBuffer, wLength);

	mLength = WideCharToMultiByte(CP_ACP, 0, pBuffer, wLength, NULL, 0, NULL, NULL);
	char* buffer = new char[mLength + 1];
	memset(buffer, 0x00, sizeof(char) * (mLength + 1));
	mLength = WideCharToMultiByte(CP_ACP, 0, pBuffer, wLength, buffer, mLength, NULL, NULL);
	buffer[mLength] = 0;

	if (pBuffer != 0 ){
		delete[] pBuffer;
	}

	return buffer;
}