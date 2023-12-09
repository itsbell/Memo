//ANSIConverter.h
#ifndef _ANSICONVERTER_H
#define _ANSICONVERTER_H

#include "TextConverter.h"

class ANSIConverter : public TextConverter {

	public:
		ANSIConverter(Document* document);
		ANSIConverter(Document* document, TCHAR* data, Long fileLength);
		virtual ~ANSIConverter();

		virtual void EncodeToMultiByteChar(TCHAR* buffer, Long byte, WCHAR* pBuffer, char* data, Long* mLength);
		virtual char* Decode();
};

#endif //_ANSICONVERTER_H