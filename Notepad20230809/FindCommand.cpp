//FindCommand.cpp
#include "FindCommand.h"

#include "Document.h"
#include "DummyRow.h"
#include "FileMaker.h"
#include "Glyph.h"
#include "MultiByteCharacter.h"
#include "resource.h"
#include "SingleByteCharacter.h"
#include "TextEditor.h"
#include <afxdlgs.h>
#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FindCommand::FindCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

FindCommand::~FindCommand() {

}

void FindCommand::Execute() {

	//1. ã����̾�αװ� ������
	if (this->textEditor->findDialog == 0) {

		//1.1. �ٲٱ���̾�αװ� ������ ���ִ�.
		if (this->textEditor->findReplaceDialog != 0) {
			delete this->textEditor->findReplaceDialog;
			this->textEditor->findReplaceDialog = 0;
		}
		int flags = FR_DOWN;
		if (this->textEditor->matchCase) {
			flags |= FR_MATCHCASE;
		}
		if (this->textEditor->matchWholeWord) {
			flags |= FR_WHOLEWORD;
		}
		//1.2. ã����̾�α׸� �����.
		this->textEditor->findDialog = new CFindReplaceDialog;
		// ���õ� ������ ���� ���� ���õ� ������ ã����̾�α��� ã�� ���뿡 ������ �ؾ��Ѵ�.
		// 1. ���õǾ� �ִ��� Ȯ���Ѵ�.
		// 2. ���õ� �����̸�
		//  2.1. ���õ� ������ �� �� �̸��̰�, 64�� �̸��̸�
		//  2.2. ���õ� ������ CString���� ��ȯ�Ѵ�.
		Document* document = this->textEditor->document;
		if (document->isSelecting == true &&
			document->endPosition.row - document->startPosition.row == 0 &&
			document->endPosition.column - document->startPosition.column <= 64) {
			Long i = 0;
			Long j;
			Long count = 0;
			Glyph* row = 0;
			Glyph* character;
			string findWhat;
			//���û����� ������ ���� ���̳��� ������
			if (document->endPosition.row >= document->GetStart() && document->endPosition.row <= document->GetStart() + document->paper->GetRowCount() - 1) {
				while (i < document->paper->GetLength() && count < document->endPosition.row - (document->GetStart() - 1)) {
					row = document->paper->GetAt(i);
					if (!dynamic_cast<DummyRow*>(row)) {
						count++;
					}
					i++;
				}
				count = document->startPosition.column;
				while (i < document->paper->GetLength() && row->GetLength() <= count) {
					count -= row->GetLength();
					row = document->paper->GetAt(i);
					i++;
				}
				j = count;
				count = document->endPosition.column - document->startPosition.column;
				while (j < row->GetLength() && count > 0) {
					character = row->GetAt(j);
					if (dynamic_cast<SingleByteCharacter*>(character)) {
						findWhat += character->GetCharacter();
					}
					else {
						findWhat += character->GetCharacters()[0];
						findWhat += character->GetCharacters()[1];
					}
					count--;
					j++;
					if (j == row->GetLength() && count > 0) {
						row = document->paper->GetAt(i);
						i++;
						j = 0;
					}
				}
			}

			//���õ� ���� ���̹ۿ� ������
			else {
				char buffer[MAX];
				Long byte;
				FILE* file;
				FileMaker fileMaker(this->textEditor);
				file = fopen(fileMaker.GetFile(), "rt");
				if (file != NULL) {
					memset(buffer, 0, MAX);
					fseek(file, (document->endPosition.row - 1) * (12 + MAX), SEEK_SET);
					fscanf(file, "%d", &byte);
					fseek(file, 8, SEEK_CUR);
					fread(buffer, byte, 1, file);
					i = document->GetIndex(buffer, document->startPosition.column);
					j = document->GetIndex(buffer, document->endPosition.column);
					while (i < j) {
						findWhat += buffer[i];
						i++;
					}
					fclose(file);
				}
			}
			this->textEditor->findWhat.Format("%s", findWhat.c_str());
		}
		
		this->textEditor->findDialog->Create(TRUE, this->textEditor->findWhat, NULL, flags, this->textEditor);
			
	}

	//2. ã����̾�αװ� ������ ��Ŀ���� �ִ�.
	else {
		this->textEditor->findDialog->SetFocus();
	}
}

void FindCommand::Unexecute() {

}