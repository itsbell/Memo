//FileMaker.cpp
#include "FileMaker.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "Glyph.h"
#include "DummyRow.h"
#include "Document.h"
#include "TextConverter.h"
#include "UTF8BOMConverter.h"
#include "UTF16LEConverter.h"
#include "UTF16BEConverter.h"
#include "Font.h"
#include "Date.h"
#include "Time.h"
#include "resource.h"

#include <shlobj_core.h>
#include <direct.h>
#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


FileMaker::FileMaker(TextEditor* textEditor)
	:pathName("") {
	this->textEditor = textEditor;
	this->utf8BOM[0] = 0xEF;
	this->utf8BOM[1] = 0xBB;
	this->utf8BOM[2] = 0xBF;
	this->littleEndianBOM[0] = 0xFF;
	this->littleEndianBOM[1] = 0xFE;
	this->bigEndianBOM[0] = 0xFE;
	this->bigEndianBOM[1] = 0xFF;
	this->path = new char[128];
	this->log = new char[128];
	this->backup = new char[128];
	this->file = new char[128];
	this->del = new char[128];
	this->paste = new char[128];
	this->temp = new char[128];
	this->setting = new char[128];
	::SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path);
	strcat(this->path, "\\Memo");
	mkdir(this->path);
	sprintf(this->log, "%s\\Logs", this->path);
	mkdir(this->log);
	Date date = date.Today();
	strcat(this->log, "\\");
	strcat(this->log, (char*)date);
	strcat(this->log, " ");
	strcat(this->log, (char*)this->textEditor->time);
	strcat(this->log, ".txt");
	sprintf(this->backup, "%s\\Backup",this->path);
	mkdir(this->backup);
	sprintf(this->file, "%s\\%sfile.tmp", this->path, textEditor->time);
	sprintf(this->del, "%s\\%sdel.tmp", this->path, textEditor->time);
	sprintf(this->paste, "%s\\%spaste.tmp", this->path, textEditor->time);
	sprintf(this->temp, "%s\\%stemp.tmp", this->path, textEditor->time);
	sprintf(this->setting, "%s\\setting.tmp", this->path);
}
FileMaker::FileMaker(TextEditor* textEditor, string pathName)
	:pathName(pathName){
	this->textEditor = textEditor;
	this->utf8BOM[0] = 0xEF;
	this->utf8BOM[1] = 0xBB;
	this->utf8BOM[2] = 0xBF;
	this->littleEndianBOM[0] = 0xFF;
	this->littleEndianBOM[1] = 0xFE;
	this->bigEndianBOM[0] = 0xFE;
	this->bigEndianBOM[1] = 0xFF;
	this->path = new char[128];
	this->log = new char[128];
	this->backup = new char[128];
	this->file = new char[128];
	this->del = new char[128];
	this->paste = new char[128];
	this->temp = new char[128];
	this->setting = new char[128];
	::SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path);
	strcat(this->path, "\\Memo");
	mkdir(this->path);
	sprintf(this->log, "%s\\Logs", this->path);
	mkdir(this->log);
	Date date = date.Today();
	strcat(this->log, "\\");
	strcat(this->log, (char*)date);
	strcat(this->log, " ");
	strcat(this->log, (char*)this->textEditor->time);
	strcat(this->log, ".txt");
	sprintf(this->backup, "%s\\Backup", this->path);
	mkdir(this->backup);
	sprintf(this->file, "%s\\%sfile.tmp", this->path, textEditor->time);
	sprintf(this->del, "%s\\%sdel.tmp", this->path, textEditor->time);
	sprintf(this->paste, "%s\\%spaste.tmp", this->path, textEditor->time);
	sprintf(this->temp, "%s\\%stemp.tmp", this->path, textEditor->time);
	sprintf(this->setting, "%s\\setting.tmp", this->path);
}

FileMaker::~FileMaker() {
	
	if (this->path != 0) {
		delete this->path;
	}
	if (this->log != 0) {
		delete this->log;
	}
	if (this->backup != 0) {
		delete this->backup;
	}
	if (this->file != 0) {
		delete this->file;
	}
	if (this->del != 0) {
		delete this->del;
	}
	if (this->paste != 0) {
		delete this->paste;
	}
	if (this->temp != 0) {
		delete this->temp;
	}
	if (this->setting != 0) {
		delete this->setting;
	}
}

void FileMaker::New() {
	char contents[MAX - 2];
	FILE* temp;

	temp = fopen(this->file, "wt");
	if (temp != NULL) {
		memset(contents, 127, MAX - 2);
		fprintf(temp, "%04d %05d\n", 0, 0);
		fwrite(contents, MAX - 2, 1, temp);
		fclose(temp);
	}
}

Long FileMaker::Make(char* data){
	char contents[MAX - 2];
	Long i = 0;
	Long j;
	Long k;
	Long length = 0;
	Long rowLength;
	FILE* temp;

	temp = fopen(this->file, "wt");
	if (temp != NULL) {
		while (data[i] != '\0') {
			if (i > 0) {
				fputc('\n', temp);
			}
			j = 0;
			rowLength = 0;
			memset(contents, 127, MAX - 2);
			while (data[i] != '\0' && data[i] != '\r') {
				contents[j] = data[i];
				j++;
				i++;
			}
			k = 0;
			while (k < j) {
				if (contents[k] >= 0 && contents[k] < ASCII) {
					rowLength += this->textEditor->characterMetrics->GetWidth(contents[k]);
				}
				else {
					rowLength += this->textEditor->characterMetrics->GetWidth(128);
					k++;
				}
				k++;
			}
			fprintf(temp, "%04d %05d\n", j, rowLength);
			fwrite(contents, MAX - 2, 1, temp);
			length++;
			if (data[i] == '\r') {
				i += 2;
				if (data[i] == '\0') {
					fputc('\n', temp);
					fprintf(temp, "%04d %05d\n", 0, 0);
					memset(contents, 127, MAX - 2);
					fwrite(contents, MAX - 2, 1, temp);
					length++;
				}
			}
		}
		//빈 종이 열었을 때
		if (strcmp(data, "") == 0) {
			fprintf(temp, "%04d %05d\n", 0, 0);
			memset(contents, 127, MAX - 2);
			fwrite(contents, MAX - 2, 1, temp);
			length++;
		}
		fclose(temp);
	}

	return length;
}

void FileMaker::Save(TextConverter* textConverter){
	char* data = new char[MAX];
	TCHAR* buffer = new TCHAR[MAX];
	WCHAR* pBuffer = new WCHAR[MAX];
	Long i = 1;
	Long j;
	Long byte;
	Long mLength;
	FILE* file;
	FILE* temp;
	Glyph* row;
	string contents;

	file = fopen(pathName.c_str(), "wt");
	temp = fopen(this->file, "rt");
	if (file != NULL && temp != NULL) {
		if (dynamic_cast<UTF8BOMConverter*>(textConverter)) {
			fwrite(utf8BOM, sizeof(utf8BOM), 1, file);
		}
		while (i < this->textEditor->document->GetStart()) {
			memset(data, 0, MAX);
			memset(buffer, 0, MAX);
			memset(pBuffer, 0, MAX);
			fseek(temp, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(temp, "%d", &byte);
			fseek(temp, 8, SEEK_CUR);
			fread(buffer, byte, 1, temp);
			buffer[byte] = '\n';
			byte++;
			textConverter->EncodeToMultiByteChar(buffer, byte, pBuffer, data, &mLength);
			fwrite(data, mLength, 1, file);
			i++;
		}
		i = 1;
		while (i <= this->textEditor->document->paper->GetLength()) {
			memset(data, 0, MAX);
			memset(buffer, 0, MAX);
			memset(pBuffer, 0, MAX);
			row = this->textEditor->document->paper->GetAt(i - 1);
			contents = row->GetContents();
			while (i + 1 <= this->textEditor->document->paper->GetLength() && dynamic_cast<DummyRow*>(this->textEditor->document->paper->GetAt(i))) {
				row = this->textEditor->document->paper->GetAt(i);
				contents += row->GetContents();
				i++;
			}
			if (i < this->textEditor->document->paper->GetLength()) {
				contents += '\n';
			}
			textConverter->EncodeToMultiByteChar((TCHAR*)contents.c_str(), contents.length(), pBuffer, data, &mLength);
			fputs(data, file);
			i++;
		}
		i = this->textEditor->document->GetEnd() + 1;
		while (i <= this->textEditor->document->GetLength()) {
			memset(data, 0, MAX);
			memset(buffer, 0, MAX);
			memset(pBuffer, 0, MAX);
			fseek(temp, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(temp, "%d", &byte);
			fseek(temp, 8, SEEK_CUR);
			fread(buffer, byte, 1, temp);
			j = byte - 1;
			while (j >= 0) {
				buffer[j + 1] = buffer[j];
				j--;
			}
			buffer[0] = '\n';
			byte++;
			textConverter->EncodeToMultiByteChar(buffer, byte, pBuffer, data, &mLength);
			fwrite(data, mLength, 1, file);
			i++;
		}
		fclose(file);
		fclose(temp);
	}
	if (buffer != 0) {
		delete[] buffer;
	}
	if (pBuffer != 0) {
		delete[] pBuffer;
	}
	if (data != 0) {
		delete[] data;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////C:\ProgramData\Notepad\Backup//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////ForBackUp/////////////////////////////////////////////////////////////////////////
	Time time = time.GetCurrent();
	Date date = date.Today();
	data = new char[MAX];
	buffer = new TCHAR[MAX];
	pBuffer = new WCHAR[MAX];
	i = 1;
	pathName = "";
	pathName += this->backup;
	pathName += "\\";

	char today[11];
	sprintf(today, "%s", (char*)date);
	pathName += today;
	pathName += " ";
	pathName += this->textEditor->fileName;
	char current[14];
	
	sprintf(current, " %02d시%02d분%02d초", time.GetHour(), time.GetMin(), time.GetSec());
	pathName += current;
	pathName += ".txt";
	
	file = fopen(pathName.c_str(), "wt");
	temp = fopen(this->file, "rt");
	if (file != NULL && temp != NULL) {
		if (dynamic_cast<UTF8BOMConverter*>(textConverter)) {
			fwrite(utf8BOM, sizeof(utf8BOM), 1, file);
		}
		while (i < this->textEditor->document->GetStart()) {
			memset(data, 0, MAX);
			memset(buffer, 0, MAX);
			memset(pBuffer, 0, MAX);
			fseek(temp, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(temp, "%d", &byte);
			fseek(temp, 8, SEEK_CUR);
			fread(buffer, byte, 1, temp);
			buffer[byte] = '\n';
			byte++;
			textConverter->EncodeToMultiByteChar(buffer, byte, pBuffer, data, &mLength);
			fwrite(data, mLength, 1, file);
			i++;
		}
		i = 1;
		while (i <= this->textEditor->document->paper->GetLength()) {
			memset(data, 0, MAX);
			memset(buffer, 0, MAX);
			memset(pBuffer, 0, MAX);
			row = this->textEditor->document->paper->GetAt(i - 1);
			contents = row->GetContents();
			while (i + 1 <= this->textEditor->document->paper->GetLength() && dynamic_cast<DummyRow*>(this->textEditor->document->paper->GetAt(i))) {
				row = this->textEditor->document->paper->GetAt(i);
				contents += row->GetContents();
				i++;
			}
			if (i < this->textEditor->document->paper->GetLength()) {
				contents += '\n';
			}
			textConverter->EncodeToMultiByteChar((TCHAR*)contents.c_str(), contents.length(), pBuffer, data, &mLength);
			fputs(data, file);
			i++;
		}
		i = this->textEditor->document->GetEnd() + 1;
		while (i <= this->textEditor->document->GetLength()) {
			memset(data, 0, MAX);
			memset(buffer, 0, MAX);
			memset(pBuffer, 0, MAX);
			fseek(temp, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(temp, "%d", &byte);
			fseek(temp, 8, SEEK_CUR);
			fread(buffer, byte, 1, temp);
			j = byte - 1;
			while (j >= 0) {
				buffer[j + 1] = buffer[j];
				j--;
			}
			buffer[0] = '\n';
			byte++;
			textConverter->EncodeToMultiByteChar(buffer, byte, pBuffer, data, &mLength);
			fwrite(data, mLength, 1, file);
			i++;
		}
		fclose(file);
		fclose(temp);
	}
	if (buffer != 0) {
		delete[] buffer;
	}
	if (pBuffer != 0) {
		delete[] pBuffer;
	}
	if (data != 0) {
		delete[] data;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void FileMaker::SaveUTF16(TextConverter* textConverter){
	TCHAR* buffer = new TCHAR[MAX];
	WCHAR* pBuffer = new WCHAR[MAX];
	FILE* file;
	FILE* temp;
	Long i = 1;
	Long j;
	Long byte;
	Long wLength;
	Glyph* row;
	string contents;

	file = fopen(pathName.c_str(), "wb");
	temp = fopen(this->file, "rt");
	if (file != NULL && temp != NULL) {
		if (dynamic_cast<UTF16LEConverter*>(textConverter)) {
			fwrite(littleEndianBOM, sizeof(littleEndianBOM), 1, file);
		}
		else {
			fwrite(bigEndianBOM, sizeof(bigEndianBOM), 1, file);
		}
		while (i < this->textEditor->document->GetStart()) {
			memset(buffer, 0, MAX);
			memset(pBuffer, 0, MAX);
			fseek(temp, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(temp, "%d", &byte);
			fseek(temp, 8, SEEK_CUR);
			fread(buffer, byte, 1, temp);
			buffer[byte] = '\r';
			buffer[byte + 1] = '\n';
			byte += 2;
			textConverter->EncodeToWideChar(buffer, byte, pBuffer, &wLength);
			fwrite(pBuffer, wLength * 2, 1, file);
			i++;
		}
		i = 1;
		while (i <= this->textEditor->document->paper->GetLength()) {
			contents = "";
			memset(pBuffer, 0, MAX);
			if (i > 1) {
				contents = "\r\n";
			}
			row = this->textEditor->document->paper->GetAt(i - 1);
			contents += row->GetContents();
			while (i + 1 <= this->textEditor->document->paper->GetLength() && dynamic_cast<DummyRow*>(this->textEditor->document->paper->GetAt(i))) {
				row = this->textEditor->document->paper->GetAt(i);
				contents += row->GetContents();
				i++;
			}
			byte = contents.length();
			textConverter->EncodeToWideChar((TCHAR*)contents.c_str() , byte, pBuffer, &wLength);
			fwrite(pBuffer, wLength * 2, 1, file);
			i++;
		}
		i = this->textEditor->document->GetEnd() + 1;
		while (i <= this->textEditor->document->GetLength()) {
			memset(buffer, 0, MAX);
			memset(pBuffer, 0, MAX);
			fseek(temp, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(temp, "%d", &byte);
			fseek(temp, 8, SEEK_CUR);
			fread(buffer, byte, 1, temp);
			j = byte - 1;
			while (j >= 0) {
				buffer[j + 2] = buffer[j];
				j--;
			}
			buffer[1] = '\n';
			buffer[0] = '\r';
			byte += 2;
			textConverter->EncodeToWideChar(buffer, byte, pBuffer, &wLength);
			fwrite(pBuffer, wLength * 2, 1, file);
			i++;
		}
		fclose(file);
		fclose(temp);
	}
	if (buffer != 0) {
		delete[] buffer;
	}
	if (pBuffer != 0) {
		delete[] pBuffer;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////C:\ProgramData\Notepad\Backup//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////ForBackUp/////////////////////////////////////////////////////////////////////////
	Time time = time.GetCurrent();
	Date date = date.Today();
	pathName = "";
	pathName += this->backup;
	pathName += "\\";

	char today[11];
	sprintf(today, "%s", (char*)date);
	pathName += today;
	pathName += " ";
	pathName += this->textEditor->fileName;
	char current[14];

	sprintf(current, " %02d시%02d분%02d초", time.GetHour(), time.GetMin(), time.GetSec());
	pathName += current;
	pathName += ".txt";

	buffer = new TCHAR[MAX];
	pBuffer = new WCHAR[MAX];
	i = 1;

	file = fopen(pathName.c_str(), "wb");
	temp = fopen(this->file, "rt");
	if (file != NULL && temp != NULL) {
		if (dynamic_cast<UTF16LEConverter*>(textConverter)) {
			fwrite(littleEndianBOM, sizeof(littleEndianBOM), 1, file);
		}
		else {
			fwrite(bigEndianBOM, sizeof(bigEndianBOM), 1, file);
		}
		while (i < this->textEditor->document->GetStart()) {
			memset(buffer, 0, MAX);
			memset(pBuffer, 0, MAX);
			fseek(temp, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(temp, "%d", &byte);
			fseek(temp, 8, SEEK_CUR);
			fread(buffer, byte, 1, temp);
			buffer[byte] = '\r';
			buffer[byte + 1] = '\n';
			byte += 2;
			textConverter->EncodeToWideChar(buffer, byte, pBuffer, &wLength);
			fwrite(pBuffer, wLength * 2, 1, file);
			i++;
		}
		i = 1;
		while (i <= this->textEditor->document->paper->GetLength()) {
			contents = "";
			memset(pBuffer, 0, MAX);
			if (i > 1) {
				contents = "\r\n";
			}
			row = this->textEditor->document->paper->GetAt(i - 1);
			contents += row->GetContents();
			while (i + 1 <= this->textEditor->document->paper->GetLength() && dynamic_cast<DummyRow*>(this->textEditor->document->paper->GetAt(i))) {
				row = this->textEditor->document->paper->GetAt(i);
				contents += row->GetContents();
				i++;
			}
			byte = contents.length();
			textConverter->EncodeToWideChar((TCHAR*)contents.c_str(), byte, pBuffer, &wLength);
			fwrite(pBuffer, wLength * 2, 1, file);
			i++;
		}
		i = this->textEditor->document->GetEnd() + 1;
		while (i <= this->textEditor->document->GetLength()) {
			memset(buffer, 0, MAX);
			memset(pBuffer, 0, MAX);
			fseek(temp, (i - 1) * (12 + MAX), SEEK_SET);
			fscanf(temp, "%d", &byte);
			fseek(temp, 8, SEEK_CUR);
			fread(buffer, byte, 1, temp);
			j = byte - 1;
			while (j >= 0) {
				buffer[j + 2] = buffer[j];
				j--;
			}
			buffer[1] = '\n';
			buffer[0] = '\r';
			byte += 2;
			textConverter->EncodeToWideChar(buffer, byte, pBuffer, &wLength);
			fwrite(pBuffer, wLength * 2, 1, file);
			i++;
		}
		fclose(file);
		fclose(temp);
	}
	if (buffer != 0) {
		delete[] buffer;
	}
	if (pBuffer != 0) {
		delete[] pBuffer;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void FileMaker::Remove() {
	remove(this->paste);
	remove(this->del);
	remove(this->file);
}

void FileMaker::ConvertChar(char* buffer, Long* byte, Long index, bool start, Long* rowLength)
{
	Long i;
	Long j;
	
	*rowLength = 0;
	if (start) {
		i = index;
		*byte = index;
	}
	else {
		i = 0;
		j = index;
		while (j < *byte) {
			buffer[i] = buffer[j];
			i++;
			j++;
		}
		(*byte) -= index;
	}
	while (i < MAX) {
		buffer[i] = 127;
		i++;
	}
	i = 0;
	while (i < *byte) {
		if (buffer[i] >= 0 && buffer[i] < 127) {
			(*rowLength) += this->textEditor->characterMetrics->GetWidth(buffer[i]);
		}
		else {
			(*rowLength) += this->textEditor->characterMetrics->GetWidth(128);
			i++;
		}
		i++;
	}
}

void FileMaker::SaveSetting(Font* font, bool isWrapped){
	FILE* file;
	LOGFONT lf = font->GetLogfont();

	file = fopen(this->setting, "wt");
	if (file != NULL) {
		fprintf(file, "%05d %05d %05d %05d %05d %05d %05d %05d %05d %05d %05d %05d %05d\n",
			lf.lfHeight, lf.lfWidth, lf.lfEscapement, lf.lfOrientation, lf.lfWeight, lf.lfItalic, lf.lfUnderline,
			lf.lfStrikeOut, lf.lfCharSet, lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality, lf.lfPitchAndFamily);
		fprintf(file, "%s\n", lf.lfFaceName);
		if (isWrapped) {
			fprintf(file, "TRUE\n", file);
		}
		else {
			fprintf(file, "FALSE\n", file);
		}
		fclose(file);
	}
}

void FileMaker::RecordLog(UINT nID) {
	FILE* file;
	Date date = date.Today();
	Time time = time.GetCurrent();
	string command;
	string log;

	switch (nID) {
		case IDM_CHAR: command = "OnCharCommand"; break;
		case IDM_CHARSELECTING: command = "OnCharSelectingCommand"; break;
		case IDM_CHARENTER: command = "OnCharEnterCommand"; break;
		case IDM_CHARENTERSELECTING: command = "OnCharEnterSelectingCommand"; break;
		case IDM_IMECOMPOSITION: command = "OnImeCompositionCommand"; break;
		case IDM_IMECOMPOSITIONSELECTING: command = "OnImeCompositionSelectingCommand"; break;
		case IDM_IMECHAR: command = "OnImeCharCommand"; break;
		case IDM_BACKSPACEKEY: command = "BackSpaceKeyCommand"; break;
		case IDM_CHARACTERBACKSPACEKEY: command = "CharacterBackSpaceKeyCommand"; break;
		case IDM_ROWBACKSPACEKEY: command = "RowBackSpaceKeyCommand"; break;
		case IDM_SELECTINGBACKSPACEKEY: command = "SelectingBackSpaceKeyCommand"; break;
		case IDM_DELETEKEY: command = "DeleteKeyCommand"; break;
		case IDM_CHARACTERDELETEKEY: command = "CharacterDeleteKeyCommand"; break;
		case IDM_ROWDELETEKEY: command = "RowDeleteKeyCommand"; break;
		case IDM_FINDREPLACE: command = "FindReplaceCommand"; break;
		case IDM_REPLACEFIND: command = "ReplaceFindCommand"; break;
		case IDM_REPLACEALL: command = "ReplaceAllCommand"; break;
		case IDM_EDIT_UNDO: command = "UnDoCommand"; break;
		case IDM_EDIT_REDO: command = "ReDoCommand"; break;
		case IDM_EDIT_CUT: if (this->textEditor->document->isSelecting == true) { command = "CutCommand"; } break;
		case IDM_EDIT_COPY: command = "CopyCommand"; break;
		case IDM_EDIT_PASTE: 
			if (this->textEditor->document->isSelecting == false) {
				command = "PasteCommand"; break;
			}
			else {
				command = "PasteSelectingCommand"; break;
			}
		case IDM_EDIT_DELETE: command = "DeleteCommand"; break;
		case IDM_EDIT_FIND: command = "FindCommand"; break;
		case IDM_EDIT_FINDNEXT: command = "FindNextCommand"; break;
		case IDM_EDIT_FINDPREVIOUS: command = "FindPreviousCommand"; break;
		case IDM_EDIT_REPLACE: command = "ReplaceCommand"; break;
		case IDM_EDIT_MOVE: command = "MoveCommand"; break;
		case IDM_EDIT_SELECTALL: command = "SelectAllCommand"; break;
		case IDM_EDIT_DATETIME: 
			if (this->textEditor->document->isSelecting == false) {
				command = "DateTimeCommand"; break;
			}
			else {
				command = "SelectingDateTimeCommand"; break;
			}
		case IDM_EDIT_FONT: command = "FontCommand"; break;
		case IDM_VIEW_WRAP: command = "WrapCommand"; break;
		case IDM_VIEW_ZOOMIN: command = "ZoomInCommand"; break;
		case IDM_VIEW_ZOOMOUT: command = "ZoomOutCommand"; break;
		case IDM_VIEW_RESET: command = "ZoomResetCommand"; break;
		case IDM_LBUTTONDOWN: command = "LButtonDownCommand"; break;
		case IDM_LBUTTONUP: command = "LButtonUpCommand"; break;
		case IDM_RBUTTONUP: command = "RButtonUpCommand"; break;
		case IDM_MOUSEMOVE: command = "MouseMoveCommand"; break;
		case IDM_MOUSEWHEEL: command = "MouseWheelCommand"; break;
		default: break;
	}
	if (command != "") {
		log += (char*)date;
		char timeFormat[11];
		sprintf(timeFormat, " %02d:%02d:%02d ", time.GetHour(), time.GetMin(), time.GetSec());
		log += timeFormat;
		log += command;
	}
	file = fopen(this->log, "at");
	if (file != NULL) {
		fprintf(file, "%s\n", log.c_str());
		fclose(file);
	}
}
