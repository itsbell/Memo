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

	//1. 찾기다이얼로그가 없으면
	if (this->textEditor->findDialog == 0) {

		//1.1. 바꾸기다이얼로그가 있으면 없애다.
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
		//1.2. 찾기다이얼로그를 만들다.
		this->textEditor->findDialog = new CFindReplaceDialog;
		// 선택된 내용이 있을 때는 선택된 내용이 찾기다이얼로그의 찾을 내용에 들어가도록 해야한다.
		// 1. 선택되어 있는지 확인한다.
		// 2. 선택된 상태이면
		//  2.1. 선택된 내용이 한 줄 미만이고, 64자 미만이면
		//  2.2. 선택된 내용을 CString으로 변환한다.
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
			//선택상태인 마지막 줄이 종이내에 있으면
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

			//선택된 줄이 종이밖에 있으면
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

	//2. 찾기다이얼로그가 있으면 포커스를 주다.
	else {
		this->textEditor->findDialog->SetFocus();
	}
}

void FindCommand::Unexecute() {

}