//UTF8BOMConverter.h
#ifndef _UTF8BOMCONVERTER_H
#define _UTF8BOMCONVERTER_H

#include "TextConverter.h"

class UTF8BOMConverter : public TextConverter{

	public:
		UTF8BOMConverter(Document* document);
		UTF8BOMConverter(Document* document, TCHAR* data, Long fileLength);
		virtual ~UTF8BOMConverter();

		virtual void EncodeToMultiByteChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, char* data, Long* mLength);
		virtual char* Decode();
};

#endif //_UTF8BOMCONVERTER_H