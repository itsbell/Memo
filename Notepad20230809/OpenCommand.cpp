//OpenCommand.cpp

#include "TextEditor.h"
#include "OpenCommand.h"
#include "Glyph.h"
#include "Paper.h"
#include "ScrollController.h"
#include "Document.h"
#include "CharacterMetrics.h"
#include "Stack.h"
#include "CFileDialogEx.h"
#include "TextConverterFactory.h"
#include "TextConverter.h"
#include "FileMaker.h"
#include "ANSIConverter.h"
#include "UTF8Converter.h"
#include "UTF8BOMConverter.h"
#include "UTF16LEConverter.h"
#include "UTF16BEConverter.h"
#include "resource.h"
#include <afxdlgs.h>
#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Ŭ������: OpenCommand
��    ��: ���� OpenCommand -�� FileCommand -�� Command / ������ ����. / ���� ���������� : Command
*/


OpenCommand::OpenCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

OpenCommand::~OpenCommand() {

}

/*
�Լ��̸�: Execute
��    ��: ������ ����.
��    ��: ����
��    ��: ����
*/
void OpenCommand::Execute() {
	int load;
	int save;
	bool doLoad = true;
	CString pathName;
	CString fileName;
	TCHAR szFilters[] = "�ؽ�Ʈ ����(*.txt)|*.txt|";

	//1. ������ ������ ������
	if (this->textEditor->isModified) {
		int ret;
		CString message;

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
				this->textEditor->fileName = fileName;
				this->textEditor->pathName = pathName;
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
				
				this->textEditor->isModified = false;
			}
			//1.2.3. '���'�� ������ '����' flag�� ��ġ��.
			else {
				doLoad = false;
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
		}
		//1.4. ��Ҹ� ������ '����' flag�� ��ġ��.
		else if (ret == IDCANCEL) {
			doLoad = false;
		}
	}

	//2. '����' flag�� ���̸�
	if (doLoad) {
		CFileDialogEx loadFileDialog(TRUE, "txt", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, NULL, 0, FALSE);
		load = loadFileDialog.DoModal();
		if (load == IDOK) {
			if (this->textEditor->note != 0) {
				delete this->textEditor->note;
			}
			this->textEditor->note = new Paper;
			this->textEditor->document->paper = this->textEditor->note;

			pathName = loadFileDialog.GetPathName();
			fileName = loadFileDialog.GetFileTitle();
			memset(this->textEditor->encoding, 0, 16);
			strcpy(this->textEditor->encoding, loadFileDialog.encoding);
		
			CFile file;
			if (file.Open(pathName, CFile::modeRead)) {
				Long fileLength = (Long)file.GetLength();
				TCHAR* pData = new TCHAR[fileLength + 2];
				memset(pData, 0, fileLength + 2);
				LONG readSize = file.Read(pData, fileLength);
				if (readSize == fileLength) {
					TextConverterFactory textConverterFactory(this->textEditor->document);
					TextConverter* textConverter;
					if (strcmp(this->textEditor->encoding, "�ڵ� �˻�") == 0) {
						textConverter = textConverterFactory.Create(pData, fileLength);
					}
					else {
						textConverter = textConverterFactory.Create(this->textEditor->encoding, pData, fileLength);
					}
					char* buffer = textConverter->Decode();
					FileMaker fileMaker(this->textEditor);
					fileMaker.Remove();
					Long count = fileMaker.Make(buffer);
					this->textEditor->document->SetLength(count);
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
					if (pData != 0) {
						delete[] pData;
					}
					if (buffer != 0) {
						delete[] buffer;
					}
				}
			}
			file.Close();
			FileMaker fm(this->textEditor);
			if (this->textEditor->isWrapped) {
				this->textEditor->document->LoadToMemory(1, OPTIMAL, this->textEditor->characterMetrics, this->textEditor->rect.right, &fm);
			}
			else {
				this->textEditor->document->LoadToMemory(1, OPTIMAL, this->textEditor->characterMetrics, &fm);
			}
			this->textEditor->note->First();
			this->textEditor->current = this->textEditor->note->GetAt(0);
			this->textEditor->current->First();
			this->textEditor->document->SetStart(1);
			this->textEditor->document->SetEnd(this->textEditor->note->GetRowCount());
			this->textEditor->isUpdated = true;
			this->textEditor->isScrolling = false;
			this->textEditor->document->isSelecting = false;
			
			this->textEditor->GetParent()->SetWindowText(fileName + " - �޸���");

			this->textEditor->fileName = fileName;
			this->textEditor->pathName = pathName;
			this->textEditor->isModified = false;

			this->textEditor->x = 0;
			this->textEditor->y = 0;
			this->textEditor->isMoving = false;
			this->textEditor->destination = 0;
			
			this->textEditor->scrollController->UpdateFileVSInfo(true);
			this->textEditor->scrollFlags = SF_ALL;
			this->textEditor->scrollController->UpdateMaximum();
			this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
			this->textEditor->Notify();
			this->textEditor->Invalidate(FALSE);

			if (this->textEditor->unDoCommands != 0) {
				delete this->textEditor->unDoCommands;
				this->textEditor->unDoCommands = 0;
			}
			if (this->textEditor->reDoCommands != 0) {
				delete this->textEditor->reDoCommands;
				this->textEditor->reDoCommands = 0;
			}
			if (this->textEditor->findDialog != 0) {
				delete this->textEditor->findDialog;
				this->textEditor->findDialog = 0;
			}
			if (this->textEditor->findReplaceDialog != 0) {
				delete this->textEditor->findReplaceDialog;
				this->textEditor->findReplaceDialog = 0;
			}
		}
	}
}

void OpenCommand::Unexecute() {

}