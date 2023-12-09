//WrapCommand.cpp
#include "WrapCommand.h"
#include "TextEditor.h"
#include "Glyph.h"
#include <afxwin.h>
#include "ScrollController.h"
#include "HorizontalScroll.h"
#include "VerticalScroll.h"
#include "FileMaker.h"
#include "MemoryController.h"
#include "Document.h"
#include "Stack.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

WrapCommand::WrapCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

WrapCommand::~WrapCommand() {

}

void WrapCommand::Execute() {
	CMenu* pMenu = this->textEditor->GetParent()->GetMenu();
	UINT state = pMenu->GetMenuState(IDM_VIEW_WRAP, MF_BYCOMMAND);
	FileMaker fm(this->textEditor);
	//1. ���̿��� ���������ϴ�.
	this->textEditor->isMoving = false;
	this->textEditor->document->isSelecting = false;	
	
	if (this->textEditor->isUpdated == false) {
		this->textEditor->memoryController->Save();
	}
	//2. �ڵ��ٹٲ� �޴��� üũ���� ���� �����̸�
	if (state == MF_UNCHECKED) {
		//3.1. �ڵ��ٹٲ� �޴��� üũ�ϴ�.
		pMenu->CheckMenuItem(IDM_VIEW_WRAP, MF_CHECKED);
		//3.2. �������� �ڵ��ٹٲ� �ϴ�.
		this->textEditor->document->Wrap(this->textEditor->rect.right, this->textEditor->characterMetrics, &fm);
		//3.3. �ڵ��ٹٲ� ���¸� ��ġ��.
		this->textEditor->isWrapped = true;
	}

	//4. �ڵ��ٹٲ� �޴��� üũ�� �����̸�
	else {
		//4.1. �ڵ��ٹٲ� �޴��� üũ�����ϴ�.
		pMenu->CheckMenuItem(IDM_VIEW_WRAP, MF_UNCHECKED);
		//4.2. ���̿��� �ڵ��ٹٲ��� �����ϴ�.
		this->textEditor->document->UnWrap(this->textEditor->characterMetrics, &fm);
		//4.3. �ڵ��ٹٲ� ���¸� ��ġ��.
		this->textEditor->isWrapped = false;
	}

	//5. ���̿��� ó������ �̵��ϴ�.
	this->textEditor->isScrolling = false;
	this->textEditor->note = this->textEditor->document->paper;
	this->textEditor->note->First();
	this->textEditor->current = this->textEditor->note->GetAt(0);
	this->textEditor->current->First();
	this->textEditor->x = 0;
	this->textEditor->y = 0;
	if (this->textEditor->unDoCommands != 0) {
		delete this->textEditor->unDoCommands;
		this->textEditor->unDoCommands = 0;
	}
	if (this->textEditor->reDoCommands != 0) {
		delete this->textEditor->reDoCommands;
		this->textEditor->reDoCommands = 0;
	}

	this->textEditor->scrollController->UpdateFileVSInfo(true);
	if (this->textEditor->isWrapped == true) {
		this->textEditor->scrollFlags = SF_WRAP;
	}
	else {
		this->textEditor->scrollFlags = SF_ALL;
	}
	this->textEditor->scrollController->UpdateMaximum();
	this->textEditor->scrollController->horizontalScroll->SetPosition(0);
	this->textEditor->scrollController->verticalScroll->SetPosition(0);
	this->textEditor->Notify();
	this->textEditor->Invalidate(FALSE);
}

void WrapCommand::Unexecute() {

}