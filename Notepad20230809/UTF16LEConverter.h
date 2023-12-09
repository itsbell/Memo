//UTF16LEConverter.h
#ifndef _UTF16LECONVERTER_H
#define _UTF16LECONVERTER_H

#include "TextConverter.h"

class UTF16LEConverter :public TextConverter {

	public:
		UTF16LEConverter(Document* document);
		UTF16LEConverter(Document* document, TCHAR* data, Long fileLength);
		virtual ~UTF16LEConverter();

		virtual void EncodeToWideChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, Long* wLength);
		virtual char* Decode();
};

#endif //_UTF16LECONVERTER_H