//TextConverter.h
#ifndef _TEXTCONVERTERFACTORY_H
#define _TEXTCONVERTERFACTORY_H

#include <afxwin.h>
typedef signed long int Long;

class Document;
class TextConverter;
class TextConverterFactory {

	public:
		Document* document;

	public:
		TextConverterFactory(Document* document);
		~TextConverterFactory();

		TextConverter* Create(char* encoding);
		TextConverter* Create(char* encoding, TCHAR* data, Long fileLength);
		TextConverter* Create(TCHAR* data, Long fileLength);
		bool IsUTF8(const void* data, Long fileLength);

	private:
		unsigned char littleEndianBOM[2];
		unsigned char bigEndianBOM[2];
		unsigned char utf8BOM[3];
};

#endif //_TEXTCONVERTERFACTORY_H