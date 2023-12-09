//CloseCommand.cpp
#include "CloseCommand.h"
#include "Notepad.h"
#include "TextEditor.h"
#include "TextConverterFactory.h"
#include "FileMaker.h"
#include "CFileDialogEx.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CloseCommand::CloseCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

CloseCommand::~CloseCommand() {

}

void CloseCommand::Execute() {

	//1. ������ ������ ������
	if (this->textEditor->isModified) {
		int ret;
		int save;
		CString message;
		CString pathName;
		CString fileName;
		TCHAR szFilters[] = "�ؽ�Ʈ ����(*.txt)|*.txt|";

		//1.1. �޽����ڽ��� ����.
		if (this->textEditor->pathName != "") {
			message.Format("���� ������ %s�� �����Ͻðڽ��ϱ�?", this->textEditor->pathName.c_str());
		}
		else if (this->textEditor->pathName == "") {
			message.Format("���� ������ %s�� �����Ͻðڽ��ϱ�?", this->textEditor->fileName.c_str());
		}
		ret = this->textEditor->MessageBox(message, "�޸���", MB_YESNOCANCEL);

		//1.2. ��ΰ� ������ ���� �ʰ� '��'�� ���� ���
		if (this->textEditor->pathName == "" && ret == IDYES) {
			//1.2.1. ���� �����ȭ���ڸ� ����.
			CFileDialogEx saveAsFileDialog(FALSE, "txt", "*.txt", OFN_OVERWRITEPROMPT, szFilters, NULL, 0, FALSE);
			save = saveAsFileDialog.DoModal();

			if (save == IDOK) {
				fileName = saveAsFileDialog.GetFileTitle();
				pathName = saveAsFileDialog.GetPathName();
				memset(this->textEditor->encoding, 0, 16);
				strcpy(this->textEditor->encoding, saveAsFileDialog.encoding);
				FileMaker fileMaker(this->textEditor, (LPCTSTR)pathName);
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
				this->textEditor->GetParent()->SetWindowText(fileName + " - �޸���");
				//1.2.2.5. �����̸�, ���ϰ��, �������θ� ��ġ��.
				this->textEditor->fileName = fileName;
				this->textEditor->pathName = pathName;
				this->textEditor->isModified = false;
				dynamic_cast<Notepad*>(this->textEditor->GetParent())->exit = true;
			}
		}
		//1.3. ����� ��ΰ� �ְ� '��'�� ���� ���
		else if (this->textEditor->pathName != "" && ret == IDYES) {
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
			this->textEditor->GetParent()->SetWindowText((this->textEditor->fileName + " - �޸���").c_str());
			this->textEditor->isModified = false;
			dynamic_cast<Notepad*>(this->textEditor->GetParent())->exit = true;
		}
		else if (ret == IDNO) {
			dynamic_cast<Notepad*>(this->textEditor->GetParent())->exit = true;
		}
	}
	else {
		dynamic_cast<Notepad*>(this->textEditor->GetParent())->exit = true;
	}
}

void CloseCommand::Unexecute() {

}