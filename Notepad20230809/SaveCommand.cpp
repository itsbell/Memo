//SaveCommand.cpp

#include "SaveCommand.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "Document.h"
#include "CFileDialogEx.h"
#include "FileMaker.h"
#include "TextConverter.h"
#include "TextConverterFactory.h"
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

SaveCommand::SaveCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

SaveCommand::~SaveCommand() {

}

void SaveCommand::Execute() {
	
	//1. ���ϰ�ΰ� ���� ���
	if (this->textEditor->pathName.compare(string("")) == 0 ) {
		int save;
		TCHAR szFilters[] = "�ؽ�Ʈ ����(*.txt)|*.txt|";
		CFileDialogEx fileDialog(FALSE, "txt", "*.txt", OFN_OVERWRITEPROMPT, szFilters, NULL, 0, FALSE);

		save = fileDialog.DoModal();
		if (save == IDOK) {
			CString fileName = fileDialog.GetFileTitle();
			CString pathName = fileDialog.GetPathName();
			this->textEditor->fileName = fileName;
			this->textEditor->pathName = pathName;
			memset(this->textEditor->encoding, 0, 16);
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
			
			// ĸ���� ��ġ��.
			this->textEditor->GetParent()->SetWindowText(fileName + " - �޸���");
			// �����̸�, ���ϰ��, ���������� ��ġ��.
			
			this->textEditor->isModified = false;
		}
	}

	//2. ���� ��ΰ� �ְ� ������ ������ ������
	else if (this->textEditor->pathName != "" && this->textEditor->isModified) {
		// ������ �����ϴ�.
		FileMaker fileMaker(this->textEditor, this->textEditor->pathName);
		TextConverterFactory textConverterFactory(this->textEditor->document);
		TextConverter* textConverter = textConverterFactory.Create(this->textEditor->encoding);
		if ((strcmp(this->textEditor->encoding, "ANSI") == 0) || (strcmp(this->textEditor->encoding, "UTF-8") == 0) || (strcmp(this->textEditor->encoding, "UTF-8(BOM)") == 0)) {
			fileMaker.Save(textConverter);
		}
		else if ((strcmp(this->textEditor->encoding, "UTF-16 LE") == 0) || (strcmp(this->textEditor->encoding, "UTF-16 BE") == 0)) {
			fileMaker.SaveUTF16(textConverter);
		}
		if (textConverter != 0) {
			delete textConverter;
		}
		// ĸ���� ��ġ��.
		this->textEditor->GetParent()->SetWindowText((this->textEditor->fileName + " - �޸���").c_str());
		//���������� ��ġ��.
		this->textEditor->isModified = false;
	}
}

void SaveCommand::Unexecute() {

}