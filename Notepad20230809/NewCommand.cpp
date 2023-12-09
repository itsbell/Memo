//NewCommand.cpp
#include "NewCommand.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "ScrollController.h"
#include "resource.h"
#include "Document.h"
#include "Stack.h"
#include "FileMaker.h"
#include "TextConverter.h"
#include "TextConverterFactory.h"

#include <afxdlgs.h>
#pragma warning (disable: 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

NewCommand::NewCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

NewCommand::~NewCommand() {

}

void NewCommand::Execute() {
	int save;
	int answer;
	bool newPaper = true;
	Long index;
	CString message;
	GlyphFactory glyphFactory;

	if (this->textEditor->isModified == true) {	
		message.Format("���� ������ %s�� �����Ͻðڽ��ϱ�?", this->textEditor->fileName.c_str());
		answer = this->textEditor->MessageBox(message, "�޸���", MB_YESNOCANCEL);
		if (answer == IDYES) {
			//1. ���ϰ�ΰ� ���� ���
			if (this->textEditor->pathName.compare(string("")) == 0) {
				//1.1. ���� �����ȭ���ڸ� ����.
				TCHAR szFilters[] = _T("�ؽ�Ʈ ����(*.txt)|*.txt|");
				CFileDialog fileDialog(FALSE, "txt", "*.txt", OFN_OVERWRITEPROMPT, szFilters, NULL, 0, TRUE);
				save = fileDialog.DoModal();
				if (save == IDOK) {
					CString fileName = fileDialog.GetFileTitle();
					CString pathName = fileDialog.GetPathName();
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
					this->textEditor->fileName = fileName;
					this->textEditor->pathName = pathName;
					this->textEditor->isModified = false;
				}
				else {
					newPaper = false;
				}
			}

			//2. ���� ��ΰ� �ְ� ������ ������ ������
			else if (this->textEditor->pathName != "" && this->textEditor->isModified == true) {
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
		}
		else if (answer == IDCANCEL) {
			newPaper = false;
		}
	}

	if (newPaper == true) {
		if (this->textEditor->note != 0) {
			delete this->textEditor->note;
		}
		
		this->textEditor->note = glyphFactory.Create((char*)"\0");
		index = this->textEditor->note->Add(glyphFactory.Create((char*)"\r\n"));
		this->textEditor->current = this->textEditor->note->GetAt(index);
		this->textEditor->document->paper = this->textEditor->note;
		this->textEditor->document->SetStart(1);
		this->textEditor->document->SetEnd(1);
		this->textEditor->document->SetLength(1);
		this->textEditor->document->isSelecting = false;
		this->textEditor->document->startPosition = Position(-1, -1);
		this->textEditor->document->endPosition = Position(-1, -1);
		FileMaker fileMaker(this->textEditor);
		fileMaker.Remove();
		fileMaker.New();
		
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

		this->textEditor->isMoving = false;
		this->textEditor->isUpdated = true;
		this->textEditor->isModified = false;
		this->textEditor->isScrolling = false;
		this->textEditor->isComposing = false;
		this->textEditor->fileName = string("�������");
		this->textEditor->pathName = string("");
		this->textEditor->x = 0;
		this->textEditor->y = 0;
		this->textEditor->destination = 0;
		strcpy(this->textEditor->encoding, "UTF-8");
		this->textEditor->findWhat = "";
		this->textEditor->replaceWith = "";
		this->textEditor->matchCase = false;
		this->textEditor->matchWholeWord = false;
		this->textEditor->preview = 0;
		this->textEditor->GetParent()->SetWindowText("������� - �޸���");
		this->textEditor->scrollController->UpdateFileVSInfo(true);
		this->textEditor->scrollFlags = SF_ALL;
		this->textEditor->scrollController->UpdateMaximum();
		this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
		this->textEditor->Notify();
		this->textEditor->Invalidate(FALSE);
	}
}

void NewCommand::Unexecute() {

}
