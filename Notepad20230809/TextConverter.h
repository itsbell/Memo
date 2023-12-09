//TextConverter.h
#ifndef _TEXTCONVERTER_H
#define _TEXTCONVERTER_H

#include <afxwin.h>
#include <string>
using namespace std;
typedef signed long int Long;

class Document;
class TextConverter {

	public:
		Document* document;
		TCHAR* data;
		Long fileLength;

	public:
		TextConverter();
		virtual ~TextConverter() = 0;

		virtual void EncodeToMultiByteChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, char* data, Long* mLength);
		virtual void EncodeToWideChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, Long* wLength);
		virtual char* Decode() = 0;
};

#endif //_TEXTCONVERTER_H