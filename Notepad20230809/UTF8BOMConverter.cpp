//UTF8BOMConverter.cpp
#include "UTF8BOMConverter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UTF8BOMConverter::UTF8BOMConverter(Document* document) {
	this->document = document;
	this->data = 0;
	this->fileLength = 0;
}

UTF8BOMConverter::UTF8BOMConverter(Document* document, TCHAR* data, Long fileLength) {
	this->document = document;
	this->data = data;
	this->fileLength = fileLength;
}

UTF8BOMConverter::~UTF8BOMConverter() {

}

void UTF8BOMConverter::EncodeToMultiByteChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, char* data, Long* mLength)
{
	int wLength = MultiByteToWideChar(CP_ACP, 0, (char*)buffer, byte, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, (char*)buffer, byte, pBuffer, wLength);
	*mLength = WideCharToMultiByte(CP_UTF8, 0, pBuffer, wLength, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, pBuffer, wLength, data, *mLength, NULL, NULL);
}

char* UTF8BOMConverter::Decode() {
	int wLength;
	int mLength;
	WCHAR* pBuffer = new WCHAR[fileLength + 2];

	wLength = MultiByteToWideChar(CP_UTF8, 0, (char*)data + 3, fileLength - 3, NULL, NULL);
	memset(pBuffer, 0, sizeof(WCHAR) * (wLength + 1));
	MultiByteToWideChar(CP_UTF8, 0, (char*)data + 3, fileLength - 3, pBuffer, wLength);

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