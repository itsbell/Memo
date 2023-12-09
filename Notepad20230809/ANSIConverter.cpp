//ANSIConverter.cpp
#include "ANSIConverter.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ANSIConverter::ANSIConverter(Document* document){
	this->document = document;
	this->data = 0;
	this->fileLength = 0;
}

ANSIConverter::ANSIConverter(Document* document, TCHAR* data, Long fileLength) {
	this->document = document;
	this->data = data;
	this->fileLength = fileLength;
}

ANSIConverter::~ANSIConverter() {

}

void ANSIConverter::EncodeToMultiByteChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, char* data, Long* mLength) {
	strcpy(data, (char*)buffer);
	*mLength = byte;
}

char* ANSIConverter::Decode() {
	int wLength;
	int mLength;
	WCHAR* pBuffer = new WCHAR[fileLength + 2];
	
	wLength = MultiByteToWideChar(CP_ACP, 0, (char*)data, fileLength, NULL, NULL);
	memset(pBuffer, 0, sizeof(WCHAR) * (wLength + 1));
	MultiByteToWideChar(CP_ACP, 0, (char*)data, fileLength, pBuffer, wLength);

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