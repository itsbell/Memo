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

	//1. 수정한 내용이 있으면
	if (this->textEditor->isModified) {
		int ret;
		int save;
		CString message;
		CString pathName;
		CString fileName;
		TCHAR szFilters[] = "텍스트 문서(*.txt)|*.txt|";

		//1.1. 메시지박스를 띄우다.
		if (this->textEditor->pathName != "") {
			message.Format("변경 내용을 %s에 저장하시겠습니까?", this->textEditor->pathName.c_str());
		}
		else if (this->textEditor->pathName == "") {
			message.Format("변경 내용을 %s에 저장하시겠습니까?", this->textEditor->fileName.c_str());
		}
		ret = this->textEditor->MessageBox(message, "메모장", MB_YESNOCANCEL);

		//1.2. 경로가 정해져 있지 않고 '예'를 누른 경우
		if (this->textEditor->pathName == "" && ret == IDYES) {
			//1.2.1. 저장 공통대화상자를 띄우다.
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
				this->textEditor->GetParent()->SetWindowText(fileName + " - 메모장");
				//1.2.2.5. 파일이름, 파일경로, 수정여부를 고치다.
				this->textEditor->fileName = fileName;
				this->textEditor->pathName = pathName;
				this->textEditor->isModified = false;
				dynamic_cast<Notepad*>(this->textEditor->GetParent())->exit = true;
			}
		}
		//1.3. 저장된 경로가 있고 '예'를 누른 경우
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
			this->textEditor->GetParent()->SetWindowText((this->textEditor->fileName + " - 메모장").c_str());
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