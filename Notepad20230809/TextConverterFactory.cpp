//TextConverterFactory.cpp
#include "TextConverterFactory.h"
#include "ANSIConverter.h"
#include "UTF8Converter.h"
#include "UTF8BOMConverter.h"
#include "UTF16LEConverter.h"
#include "UTF16BEConverter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TextConverterFactory::TextConverterFactory(Document* document) {
	this->document = document;
	this->utf8BOM[0] = 0xEF;
	this->utf8BOM[1] = 0xBB;
	this->utf8BOM[2] = 0xBF;
	this->littleEndianBOM[0] = 0xFF;
	this->littleEndianBOM[1] = 0xFE;
	this->bigEndianBOM[0] = 0xFE;
	this->bigEndianBOM[1] = 0xFF;
}

TextConverterFactory::~TextConverterFactory() {

}

TextConverter* TextConverterFactory::Create(char* encoding) {
	TextConverter* textConverter = 0;

	if (strcmp(encoding, "ANSI") == 0) {
		textConverter = new ANSIConverter(this->document);
	}
	else if (strcmp(encoding, "UTF-8") == 0) {
		textConverter = new UTF8Converter(this->document);
	}
	else if (strcmp(encoding, "UTF-8(BOM)") == 0) {
		textConverter = new UTF8BOMConverter(this->document);
	}
	else if (strcmp(encoding, "UTF-16 LE") == 0) {
		textConverter = new UTF16LEConverter(this->document);
	}
	else if (strcmp(encoding, "UTF-16 BE") == 0) {
		textConverter = new UTF16BEConverter(this->document);
	}

	return textConverter;
}

TextConverter* TextConverterFactory::Create(char* encoding, TCHAR* data, Long fileLength) {
	TextConverter* textConverter = 0;

	if (strcmp(encoding, "ANSI") == 0) {
		textConverter = new ANSIConverter(this->document, data, fileLength);
	}
	else if (strcmp(encoding, "UTF-8") == 0) {
		textConverter = new UTF8Converter(this->document, data, fileLength);
	}
	else if (strcmp(encoding, "UTF-8(BOM)") == 0) {
		textConverter = new UTF8BOMConverter(this->document, data, fileLength);
	}
	else if (strcmp(encoding, "UTF-16 LE") == 0) {
		textConverter = new UTF16LEConverter(this->document, data, fileLength);
	}
	else if (strcmp(encoding, "UTF-16 BE") == 0) {
		textConverter = new UTF16BEConverter(this->document, data, fileLength);
	}

	return textConverter;
}

TextConverter* TextConverterFactory::Create(TCHAR* data, Long fileLength) {
	char BOM[3];
	unsigned char* lpHeader;
	TextConverter* textConverter = 0;
	memset(BOM, 0, 3);
	memmove(BOM, (char*)data, 3);
	lpHeader = (unsigned char*)BOM;

	if (lpHeader[0] == littleEndianBOM[0] && lpHeader[1] == littleEndianBOM[1]) {
		textConverter = new UTF16LEConverter(this->document, data, fileLength);
	}
	else if (lpHeader[0] == bigEndianBOM[0] && lpHeader[1] == bigEndianBOM[1]) {
		textConverter = new UTF16BEConverter(this->document, data, fileLength);
	}
	else if (lpHeader[0] == utf8BOM[0] && lpHeader[1] == utf8BOM[1] && lpHeader[2] == utf8BOM[2]) {
		textConverter = new UTF8BOMConverter(this->document, data, fileLength);
	}
	else if (IsUTF8((const void*)data, fileLength)) {
		textConverter = new UTF8Converter(this->document, data, fileLength);
	}
	else{
		textConverter = new ANSIConverter(this->document, data, fileLength);
	}

	return textConverter;
}

bool TextConverterFactory::IsUTF8(const void* data, Long fileLength) {
	bool utf8 = true;
	unsigned char* start = (unsigned char*)data;
	unsigned char* end = (unsigned char*)data + fileLength;

	while (start < end) {
		if (*start < 0x80) { // (10000000)[output][/output]
			start++;
		}
		else if (*start < (0xC0)) { // (11000000)
			utf8 = false;
			break;
		}
		else if (*start < (0xE0)) { // (11100000)
			if (start >= end - 1)
				break;
			if ((start[1] & (0xC0)) != 0x80) {
				utf8 = false;
				break;
			}
			start += 2;
		}
		else if (*start < (0xF0)) { // (11110000)
			if (start >= end - 2)
				break;
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80) {
				utf8 = false;
				break;
			}
			start += 3;
		}
		else {
			utf8 = false;
			break;
		}
	}
	return utf8;
}
