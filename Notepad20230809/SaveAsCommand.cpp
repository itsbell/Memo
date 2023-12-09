//SaveAsCommand.cpp
#include "SaveAsCommand.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "Document.h"
#include "CFileDialogEx.h"
#include "FileMaker.h"
#include "TextConverterFactory.h"
#include "TextConverter.h"
#include "ANSIConverter.h"
#include "UTF8Converter.h"
#include "UTF8BOMConverter.h"
#include "UTF16LEConverter.h"
#include "UTF16BEConverter.h"
#include <afxdlgs.h>
#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SaveAsCommand::SaveAsCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

SaveAsCommand::~SaveAsCommand() {

}

void SaveAsCommand::Execute() {
	TCHAR szFilters[] = "텍스트 문서(*.txt)|*.txt|";
	CFileDialogEx fileDialog(FALSE, "txt", "*.txt", OFN_OVERWRITEPROMPT, szFilters, NULL, 0, FALSE);

	if (fileDialog.DoModal() == IDOK) {
		CString fileName = fileDialog.GetFileTitle();
		CString pathName = fileDialog.GetPathName();
		strcpy(this->textEditor->encoding, fileDialog.encoding);
		FileMaker fileMaker(this->textEditor, (LPCTSTR)pathName);
		TextConverterFactory textConverterFactory(this->textEditor->document);
		TextConverter* textConverter = textConverterFactory.Create(this->textEditor->encoding);
		if ((strcmp(this->textEditor->encoding, "ANSI") == 0) || (strcmp(this->textEditor->encoding, "UTF-8") == 0) || (strcmp(this->textEditor->encoding, "UTF-8(BOM)") == 0)) {
			fileMaker.Save(textConverter);
		}
		else if ((strcmp(this->textEditor->encoding, "UTF-16 LE") == 0) || (strcmp(this->textEditor->encoding, "UTF-16 BE") == 0)) {
			fileMaker.SaveUTF16(textConverter);
		}
		if (dynamic_cast<ANSIConverter*>(textConverter)) {
			strcpy(this->textEditor->encoding, "ANSI");
		}
		else if (dynamic_cast<UTF8Converter*>(textConverter)) {
			strcpy(this->textEditor->encoding, "UTF-8");
		}
		else if (dynamic_cast<UTF8BOMConverter*>(textConverter)) {
			strcpy(this->textEditor->encoding, "UTF-8(BOM)");
		}
		else if (dynamic_cast<UTF16LEConverter*>(textConverter)) {
			strcpy(this->textEditor->encoding, "UTF-16 LE");
		}
		else if (dynamic_cast<UTF16BEConverter*>(textConverter)) {
			strcpy(this->textEditor->encoding, "UTF-16 BE");
		}
		if (textConverter != 0) {
			delete textConverter;
		}
		this->textEditor->GetParent()->SetWindowText(fileName + " - 메모장");
		this->textEditor->fileName = fileName;
		this->textEditor->pathName = pathName;
		this->textEditor->isModified = false;
		this->textEditor->Notify();
	}
}

void SaveAsCommand::Unexecute() {

}