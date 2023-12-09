//UTF16BEConverter.h
#ifndef _UTF16BECONVERTER_H
#define _UTF16BECONVERTER_H

#include "TextConverter.h"

class UTF16BEConverter : public TextConverter {

	public:
		UTF16BEConverter(Document* document);
		UTF16BEConverter(Document* document, TCHAR* data, Long fileLength);
		virtual ~UTF16BEConverter();

		virtual void EncodeToWideChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, Long* wLength);
		virtual char* Decode();
};

#endif //_UTF16BECONVERTER_H