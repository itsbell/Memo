//UTF8Converter.h
#ifndef _UTF8CONVERTER_H
#define _UTF8CONVERTER_H

#include "TextConverter.h"

class UTF8Converter : public TextConverter {

	public:
		UTF8Converter(Document* document);
		UTF8Converter(Document* document, TCHAR* data, Long fileLength);
		virtual ~UTF8Converter();

		virtual void EncodeToMultiByteChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, char* data, Long* mLength);
		virtual char* Decode();
};

#endif //_UTF8CONVERTER_H