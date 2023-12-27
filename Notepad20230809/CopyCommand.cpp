//CopyCommand.cpp
#include "CopyCommand.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "Stack.h"
#include "Document.h"
#include "FileMaker.h"
#include "resource.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CopyCommand::CopyCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = false;
}

CopyCommand::CopyCommand(TextEditor* textEditor, bool isNew, bool isMacro) {
	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = isMacro;
}

CopyCommand::~CopyCommand() {

}

void CopyCommand::Execute() {

	if (this->isNew == true) {
		if (this->textEditor->document->isSelecting) {
			char* data;
			char buffer[MAX];
			Long i = this->textEditor->document->startPosition.row;
			Long j;
			Long end = this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1;
			Long length;
			string* contents = new string;
			FileMaker fileMaker(this->textEditor);
			FILE* file;

			memset(buffer, 0, MAX);
			if (this->isMacro == true) {
				if (this->textEditor->reDoCommands != 0) {
					delete this->textEditor->reDoCommands;
					this->textEditor->reDoCommands = 0;
				}
				if (this->textEditor->unDoCommands == 0) {
					this->textEditor->unDoCommands = new Stack;
				}
				this->textEditor->unDoCommands->Push(new CopyCommand(this->textEditor, false, true));
			}
			//아래 조건식에 들어가는 경우가 없을것으로 생각된다. 선택영역이 종이밖이면 이미 scrolling중이고 업데이트는 되어 있을것이기 때문에
			if (this->textEditor->isUpdated == false && end < this->textEditor->document->endPosition.row) {
				this->textEditor->memoryController->Save();
				this->textEditor->document->SetEnd(end);
				this->textEditor->scrollController->UpdateFileVSInfo(true);
			}
			file = fopen(fileMaker.GetFile(), "rt");
			if (file != NULL) {
				while (i <= this->textEditor->document->endPosition.row && i < this->textEditor->document->GetStart()) {
					if (i > this->textEditor->document->startPosition.row) {
						*contents += "\r\n";
					}
					this->textEditor->document->Copy(i, file, buffer, &length);
					j = 0;
					while (j < length) {
						*contents += buffer[j];
						j++;
					}
					i++;
				}
				fclose(file);
			}
			while (i <= this->textEditor->document->endPosition.row && i <= end) {
				if (i > this->textEditor->document->startPosition.row) {
					*contents += "\r\n";
				}
				*contents += this->textEditor->document->Copy(i);
				i++;
			}
			file = fopen(fileMaker.GetFile(), "rt");
			if (file != NULL) {
				while (i <= this->textEditor->document->endPosition.row) {
					if (i > this->textEditor->document->startPosition.row) {
						*contents += "\r\n";
					}
					this->textEditor->document->Copy(i, file, buffer, &length);
					j = 0;
					while (j < length) {
						*contents += buffer[j];
						j++;
					}
					i++;
				}
				fclose(file);
			}

			data = strcpy(new char[(*contents).length() + 1], (*contents).c_str());
			length = strlen(data) + 1;

			HANDLE h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, length);
			char* p_data = (char*)::GlobalLock(h_data);

			if (p_data != NULL) {
				memcpy(p_data, data, length);
				::GlobalUnlock(h_data);
				if (::OpenClipboard(this->textEditor->m_hWnd)) {
					::EmptyClipboard();
					::SetClipboardData(CF_TEXT, h_data);
					::CloseClipboard();
				}
			}
			if (data != NULL) {
				delete[] data;
			}
			if (contents != NULL) {
				delete contents;
			}
		}
	}

	else {
		if (this->isMacro == true) {
			this->textEditor->unDoCommands->Push(new CopyCommand(this->textEditor, false, true));
		}
	}
}

void CopyCommand::Unexecute() {

	if (this->isMacro == true) {
		if (this->textEditor->reDoCommands == 0) {
			this->textEditor->reDoCommands = new Stack;
		}
		this->textEditor->reDoCommands->Push(new CopyCommand(this->textEditor, false, true));
	}
}