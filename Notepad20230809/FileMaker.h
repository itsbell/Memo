//FileMaker.h
#ifndef _FILEMAKER_H
#define _FILEMAKER_H

#include <string>
using namespace std;
typedef signed long int Long;

class Font;
class Document;
class CharacterMetrics;
class TextConverter;
class TextEditor;
class FileMaker {
	
	public:
		TextEditor* textEditor;

	public:
		FileMaker(TextEditor* textEditor);
		FileMaker(TextEditor* textEditor, string pathName);
		~FileMaker();

		void New();
		Long Make(char* data);
		void Save(TextConverter* textConverter);
		void SaveUTF16(TextConverter* textConverter);
		void Remove();
		void ConvertChar(char* buffer, Long* byte, Long index, bool start, Long* rowLength);
		void SaveSetting(Font* font, bool isWrapped);

		char* GetPath() const;
		char* GetLog() const;
		char* GetBackup() const;
		char* GetFile() const;
		char* GetDel() const;
		char* GetPaste() const;
		char* GetTemp() const;
		char* GetSetting() const;
		
	private:
		char* path;
		char* log;
		char* backup;
		char* file;
		char* del;
		char* paste;
		char* temp;
		char* setting;
		unsigned char utf8BOM[3];
		unsigned char littleEndianBOM[2];
		unsigned char bigEndianBOM[2];
		string pathName;
};

inline char* FileMaker::GetPath() const {
	return this->path;
}

inline char* FileMaker::GetLog() const {
	return this->log;
}

inline char* FileMaker::GetBackup() const {
	return this->backup;
}

inline char* FileMaker::GetFile() const {
	return this->file;
}

inline char* FileMaker::GetDel() const {
	return this->del;
}

inline char* FileMaker::GetPaste() const {
	return this->paste;
}

inline char* FileMaker::GetTemp() const {
	return this->temp;
}

inline char* FileMaker::GetSetting() const {
	return this->setting;
}

#endif //_FILEMAKER_H